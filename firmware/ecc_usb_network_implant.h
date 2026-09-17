#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <WebServer.h>

#include "ECL_Net.h"
#include "ECL_Usb.h"

#include "esp_netif.h"
#include "lwip/opt.h"

extern WebServer controlserver;

static const char* ECC_IMPLANT_CONFIG_PATH = "/usb_network_implant.txt";

#ifndef ECC_STATELESS_SNAT
#define ECC_STATELESS_SNAT 1
#endif

static const IPAddress ECC_IMPLANT_DEVICE_IP(192, 168, 7, 1);
static const IPAddress ECC_IMPLANT_TARGET_IP(192, 168, 7, 2);
static const IPAddress ECC_IMPLANT_NETMASK(255, 255, 255, 0);

static bool          s_implantEnabled = false;
static bool          s_implantStarted = false;
static bool          s_implantDhcpReady = false;
static bool          s_implantForwardingReady = false;
static bool          s_implantSourceNatReady = false;
static String        s_implantError;
static unsigned long s_implantLastNetworkCheck = 0;
static wl_status_t   s_implantLastWifiStatus = WL_NO_SHIELD;
static IPAddress     s_implantLastWifiIp(0, 0, 0, 0);
static IPAddress     s_implantLastWifiMask(0, 0, 0, 0);

static bool eccSaveUsbNetworkImplantConfig() {
  File file = LittleFS.open(ECC_IMPLANT_CONFIG_PATH, FILE_WRITE);
  if (!file) return false;
  const size_t written = file.println(s_implantEnabled ? "1" : "0");
  file.flush();
  file.close();
  return written > 0;
}

static void eccLoadUsbNetworkImplantConfig() {
  s_implantEnabled = false;
  if (!LittleFS.exists(ECC_IMPLANT_CONFIG_PATH)) return;

  File file = LittleFS.open(ECC_IMPLANT_CONFIG_PATH, FILE_READ);
  if (!file) return;
  String value = file.readStringUntil('\n');
  file.close();
  value.trim();
  s_implantEnabled = value == "1";
}

static uint32_t eccIpNumber(const IPAddress& ip) {
  return ((uint32_t)ip[0] << 24) |
         ((uint32_t)ip[1] << 16) |
         ((uint32_t)ip[2] << 8)  |
          (uint32_t)ip[3];
}

static bool eccSubnetsOverlap(const IPAddress& ipA, const IPAddress& maskA,
                              const IPAddress& ipB, const IPAddress& maskB) {
  const uint32_t aMask = eccIpNumber(maskA);
  const uint32_t bMask = eccIpNumber(maskB);
  const uint32_t aFirst = eccIpNumber(ipA) & aMask;
  const uint32_t bFirst = eccIpNumber(ipB) & bMask;
  const uint32_t aLast = aFirst | ~aMask;
  const uint32_t bLast = bFirst | ~bMask;
  return aFirst <= bLast && bFirst <= aLast;
}

static String eccImplantJsonEscape(const String& input) {
  String output;
  output.reserve(input.length() + 8);
  for (size_t i = 0; i < input.length(); ++i) {
    const char c = input[i];
    if (c == '"' || c == '\\') {
      output += '\\';
      output += c;
    } else if (c == '\n') {
      output += "\\n";
    } else if (c != '\r') {
      output += c;
    }
  }
  return output;
}

static void eccUsbNetBegin() {
  eccLoadUsbNetworkImplantConfig();
  s_implantStarted = false;
  s_implantDhcpReady = false;
  s_implantForwardingReady = false;
  s_implantSourceNatReady = false;
  s_implantError = "";

  if (!s_implantEnabled) return;
  if (!UsbNet.classSupported(ECL_NET_NCM)) {
    s_implantError = "CDC-NCM is unavailable in this Arduino-ESP32 build";
    return;
  }

  s_implantStarted = UsbNet.begin(ECL_NET_NCM,
                                  ECC_IMPLANT_DEVICE_IP,
                                  ECC_IMPLANT_NETMASK,
                                  ECC_IMPLANT_DEVICE_IP,
                                  ECC_IMPLANT_DEVICE_IP);
  if (!s_implantStarted) {
    s_implantError = "The USB NCM interface could not be started";
    return;
  }

  s_implantDhcpReady = UsbNet.configureDhcp(false, ECC_IMPLANT_DEVICE_IP,
                                            false, ECC_IMPLANT_DEVICE_IP);
  if (!s_implantDhcpReady) {
    s_implantError = "The USB NCM DHCP server could not be configured";
  }
}

static bool eccApplyUsbNetworkImplantForwarding() {
  s_implantForwardingReady = false;
  s_implantSourceNatReady = false;

  if (!s_implantEnabled || !s_implantStarted || !s_implantDhcpReady) return false;
  if (WiFi.status() != WL_CONNECTED ||
      WiFi.localIP() == IPAddress(0, 0, 0, 0)) {
    s_implantError = "Wi-Fi STA is not connected to the operator access point";
    return false;
  }

  const IPAddress wifiMask = WiFi.subnetMask();
  if (wifiMask == IPAddress(0, 0, 0, 0)) {
    s_implantError = "Wi-Fi STA has no valid IPv4 netmask";
    return false;
  }
  if (eccSubnetsOverlap(WiFi.localIP(), wifiMask,
                        ECC_IMPLANT_DEVICE_IP, ECC_IMPLANT_NETMASK)) {
    s_implantError = "Wi-Fi overlaps 192.168.7.0/24; use a different AP subnet";
    return false;
  }

  esp_netif_t* sta = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
  if (!sta) {
    s_implantError = "The Wi-Fi STA network interface was not found";
    return false;
  }
  UsbNet.enableNapt(false);

#if defined(IP_FORWARD) && IP_FORWARD
  if (esp_netif_set_default_netif(sta) != ESP_OK) {
    s_implantError = "The Wi-Fi STA interface could not be selected as default";
    return false;
  }

#if ECC_STATELESS_SNAT
  UsbNet.setStatelessSnat(true, ECC_IMPLANT_DEVICE_IP, ECC_IMPLANT_TARGET_IP);
  s_implantError = "";
  s_implantSourceNatReady = true;
  s_implantForwardingReady = true;
  return true;
#elif defined(IP_NAPT) && IP_NAPT
  UsbNet.setStatelessSnat(false, ECC_IMPLANT_DEVICE_IP, ECC_IMPLANT_TARGET_IP);
  if (esp_netif_napt_enable(sta) != ESP_OK) {
    s_implantError = "Source NAPT could not be enabled on the Wi-Fi interface";
    return false;
  }
  s_implantError = "";
  s_implantSourceNatReady = true;
  s_implantForwardingReady = true;
  return true;
#else
  s_implantError = "IPv4 NAPT is disabled in this build and ECC_STATELESS_SNAT is off";
  return false;
#endif

#else
  s_implantError = "IPv4 forwarding is disabled in this Arduino-ESP32 build";
  return false;
#endif
}

static void eccUsbNetApplyRuntime() {
  if (s_implantEnabled) eccApplyUsbNetworkImplantForwarding();
}

static void eccUsbNetTask() {
  if (!s_implantEnabled || !s_implantStarted) return;

  UsbNet.task();
  if (millis() - s_implantLastNetworkCheck < 1000U) return;
  s_implantLastNetworkCheck = millis();

  const wl_status_t status = WiFi.status();
  const IPAddress ip = WiFi.localIP();
  const IPAddress mask = WiFi.subnetMask();
  if (status != s_implantLastWifiStatus ||
      ip != s_implantLastWifiIp || mask != s_implantLastWifiMask) {
    s_implantLastWifiStatus = status;
    s_implantLastWifiIp = ip;
    s_implantLastWifiMask = mask;
    eccApplyUsbNetworkImplantForwarding();
  }
}

static bool eccUsbSerialConfigEnabled() {
  if (!LittleFS.exists("/usb_serial.txt")) return false;
  File f = LittleFS.open("/usb_serial.txt", FILE_READ);
  if (!f) return false;
  String v = f.readStringUntil('\n');
  f.close();
  v.trim();
  return v == "1";
}

static void handleUsbNetworkImplantStatus() {
  const IPAddress wifiIp = WiFi.localIP();
  const bool ncmSupported = UsbNet.classSupported(ECL_NET_NCM);
  const bool linkUp = s_implantStarted && UsbNet.linkUp();
  const String nextHop = wifiIp == IPAddress(0, 0, 0, 0)
                           ? String("<device-wifi-ip>")
                           : wifiIp.toString();

  String json = "{";
  json += "\"enabled\":" + String(s_implantEnabled ? "true" : "false");
  json += ",\"ncmSupported\":" + String(ncmSupported ? "true" : "false");
  json += ",\"started\":" + String(s_implantStarted ? "true" : "false");
  json += ",\"dhcpReady\":" + String(s_implantDhcpReady ? "true" : "false");
  json += ",\"linkUp\":" + String(linkUp ? "true" : "false");
  json += ",\"wifiConnected\":" +
          String(WiFi.status() == WL_CONNECTED ? "true" : "false");
  json += ",\"usbSerialEnabled\":" +
          String(eccUsbSerialConfigEnabled() ? "true" : "false");
  json += ",\"forwardingReady\":" +
          String(s_implantForwardingReady ? "true" : "false");
  json += ",\"sourceNatReady\":" +
          String(s_implantSourceNatReady ? "true" : "false");
  json += ",\"statelessSnat\":" +
          String(UsbNet.statelessSnatEnabled() ? "true" : "false");
  json += ",\"operatorIp\":\"" + UsbNet.snatOperator().toString() + "\"";
#if defined(IP_FORWARD) && IP_FORWARD
  json += ",\"ipForward\":true";
#else
  json += ",\"ipForward\":false";
#endif
#if defined(IP_NAPT) && IP_NAPT
  json += ",\"ipNapt\":true";
#else
  json += ",\"ipNapt\":false";
#endif
  json += ",\"wifiIp\":\"" + wifiIp.toString() + "\"";
  json += ",\"deviceIp\":\"" + ECC_IMPLANT_DEVICE_IP.toString() + "\"";
  json += ",\"targetIp\":\"" + ECC_IMPLANT_TARGET_IP.toString() + "\"";
  json += ",\"usbSubnet\":\"192.168.7.0/24\"";
  json += ",\"linuxRoute\":\"sudo ip route replace 192.168.7.0/24 via " + nextHop + "\"";
  json += ",\"windowsRoute\":\"route add 192.168.7.0 mask 255.255.255.0 " + nextHop + "\"";
  json += ",\"macosRoute\":\"sudo route -n add -net 192.168.7.0/24 " + nextHop + "\"";
  json += ",\"error\":\"" + eccImplantJsonEscape(s_implantError) + "\"";
  json += ",\"txSent\":" + String(UsbNet.txSent());
  json += ",\"txDropped\":" + String(UsbNet.txDropped());
  json += ",\"txStalled\":" + String(UsbNet.txStalled());
  json += ",\"rxFrames\":" + String(UsbNet.rxFrames());
  json += ",\"rxDropped\":" + String(UsbNet.rxStuck());

  ecl_ncm_diag_t diag;
  UsbNet.usbDiag(&diag);
  json += ",\"ncmCompletions\":" + String(diag.completions);
  json += ",\"ncmCanXmitFail\":" + String(diag.canXmitFail);
  json += ",\"ncmSoftRecover\":" + String(diag.softRecover);
  json += ",\"ncmHardRecover\":" + String(diag.hardRecover);
  json += ",\"ncmInFlight\":" + String(diag.inFlight);
  json += ",\"ncmReadyCount\":" + String(diag.readyCount);
  json += ",\"ncmFreeCount\":" + String(diag.freeCount);
  json += ",\"ncmEpInBusy\":" + String(diag.epInBusy);
  json += "}";

  controlserver.sendHeader("Cache-Control", "no-store");
  controlserver.send(200, "application/json", json);
}

static void handleUsbNetworkImplantSet() {
  if (!controlserver.hasArg("enabled")) {
    controlserver.send(400, "application/json",
                       "{\"ok\":false,\"message\":\"Missing enabled value\"}");
    return;
  }

  String value = controlserver.arg("enabled");
  value.toLowerCase();
  const bool enabled = value == "1" || value == "true" || value == "on";

  if (enabled && !UsbNet.classSupported(ECL_NET_NCM)) {
    controlserver.send(409, "application/json",
                       "{\"ok\":false,\"message\":\"CDC-NCM is unavailable in this build\"}");
    return;
  }

  if (enabled && eccUsbSerialConfigEnabled()) {
    controlserver.send(409, "application/json",
                       "{\"ok\":false,\"message\":\"Disable USB Serial first: the ESP32-S3 "
                       "cannot enumerate HID + USB Serial + NCM together\"}");
    return;
  }

  if (enabled == s_implantEnabled) {
    controlserver.send(200, "application/json",
                       "{\"ok\":true,\"restarting\":false}");
    return;
  }

  const bool previous = s_implantEnabled;
  s_implantEnabled = enabled;
  if (!eccSaveUsbNetworkImplantConfig()) {
    s_implantEnabled = previous;
    controlserver.send(500, "application/json",
                       "{\"ok\":false,\"message\":\"Could not save the setting\"}");
    return;
  }

  controlserver.sendHeader("Cache-Control", "no-store");
  controlserver.sendHeader("Connection", "close");
  controlserver.send(200, "application/json",
                     "{\"ok\":true,\"restarting\":true}");
  delay(500);
  ESP.restart();
}
