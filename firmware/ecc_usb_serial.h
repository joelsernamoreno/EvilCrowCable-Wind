#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <WebServer.h>

#if defined(SOC_USB_OTG_SUPPORTED) && SOC_USB_OTG_SUPPORTED && __has_include("USBCDC.h")
  #include "USBCDC.h"
  #define ECC_HAS_USB_CDC 1
#else
  #define ECC_HAS_USB_CDC 0
#endif

#ifndef ECC_ALLOW_CDC_WITH_NCM
#define ECC_ALLOW_CDC_WITH_NCM 0
#endif

extern WebServer controlserver;
extern WiFiClient clientServer;
static const char* ECC_USB_SERIAL_CONFIG_PATH = "/usb_serial.txt";
static bool s_usbSerialEnabled = false;
static bool s_usbSerialStarted = false;
static bool s_usbSerialBlocked = false;

#if ECC_HAS_USB_CDC
static USBCDC* s_usbSerialPort = nullptr;
#endif

static inline bool eccUsbSerialActive() {
#if ECC_HAS_USB_CDC
  return s_usbSerialPort != nullptr;
#else
  return false;
#endif
}

static inline bool eccUsbSerialSupported() {
  return ECC_HAS_USB_CDC ? true : false;
}

static inline void eccUsbSerialLog(const String& line) {
#if ECC_HAS_USB_CDC
  if (s_usbSerialPort) s_usbSerialPort->println(line);
#else
  (void)line;
#endif
}

static void eccUsbSerialLoadConfig() {
  s_usbSerialEnabled = false;
  if (!LittleFS.exists(ECC_USB_SERIAL_CONFIG_PATH)) return;
  File file = LittleFS.open(ECC_USB_SERIAL_CONFIG_PATH, FILE_READ);
  if (!file) return;
  String value = file.readStringUntil('\n');
  file.close();
  value.trim();
  s_usbSerialEnabled = (value == "1");
}

static bool eccUsbSerialSaveConfig() {
  File file = LittleFS.open(ECC_USB_SERIAL_CONFIG_PATH, FILE_WRITE);
  if (!file) return false;
  const size_t written = file.println(s_usbSerialEnabled ? "1" : "0");
  file.flush();
  file.close();
  return written > 0;
}

static void eccUsbSerialBegin() {
  eccUsbSerialLoadConfig();
  s_usbSerialStarted = false;
  s_usbSerialBlocked = false;
  if (!s_usbSerialEnabled) return;

  if (s_implantEnabled && !ECC_ALLOW_CDC_WITH_NCM) {
    s_usbSerialBlocked = true;
    return;
  }

#if ECC_HAS_USB_CDC
  s_usbSerialPort = new USBCDC();
  if (!s_usbSerialPort) return;
  s_usbSerialPort->begin();
  s_usbSerialPort->setTxTimeoutMs(20);
  s_usbSerialStarted = true;
#endif
}

static void eccUsbSerialTask() {
#if ECC_HAS_USB_CDC
  if (!s_usbSerialPort) return;

  uint8_t buf[256];
  size_t budget;
  budget = 2048;
  while (s_usbSerialPort->available() > 0 && budget > 0) {
    size_t avail = (size_t)s_usbSerialPort->available();
    if (avail > sizeof(buf)) avail = sizeof(buf);
    if (avail > budget) avail = budget;
    const size_t n = s_usbSerialPort->readBytes(buf, avail);
    if (n == 0) break;
    budget -= n;
    if (clientServer.connected()) clientServer.write(buf, n);
  }
  budget = 2048;
  while (clientServer.available() > 0 && budget > 0) {
    size_t avail = (size_t)clientServer.available();
    if (avail > sizeof(buf)) avail = sizeof(buf);
    if (avail > budget) avail = budget;
    const size_t n = clientServer.readBytes(buf, avail);
    if (n == 0) break;
    budget -= n;
    s_usbSerialPort->write(buf, n);
  }
#endif
}

static void handleUsbSerialStatus() {
  bool cdcConnected = false;
#if ECC_HAS_USB_CDC
  if (s_usbSerialPort) cdcConnected = (bool)(*s_usbSerialPort);
#endif

  String json = "{";
  json += "\"enabled\":" + String(s_usbSerialEnabled ? "true" : "false");
  json += ",\"supported\":" + String(eccUsbSerialSupported() ? "true" : "false");
  json += ",\"started\":" + String(s_usbSerialStarted ? "true" : "false");
  json += ",\"blocked\":" + String(s_usbSerialBlocked ? "true" : "false");
  json += ",\"hostConnected\":" + String(cdcConnected ? "true" : "false");
  json += ",\"tcpConnected\":" + String(clientServer.connected() ? "true" : "false");
  json += ",\"implantEnabled\":" + String(s_implantEnabled ? "true" : "false");

#if defined(ARDUINO_USB_CDC_ON_BOOT) && ARDUINO_USB_CDC_ON_BOOT
  json += ",\"cdcOnBoot\":true";
#else
  json += ",\"cdcOnBoot\":false";
#endif
  json += "}";

  controlserver.sendHeader("Cache-Control", "no-store");
  controlserver.send(200, "application/json", json);
}

static void handleUsbSerialSet() {
  if (!controlserver.hasArg("enabled")) {
    controlserver.send(400, "application/json",
                       "{\"ok\":false,\"message\":\"Missing enabled value\"}");
    return;
  }

  String value = controlserver.arg("enabled");
  value.toLowerCase();
  const bool enabled = (value == "1" || value == "true" || value == "on");

  if (enabled && !eccUsbSerialSupported()) {
    controlserver.send(409, "application/json",
                       "{\"ok\":false,\"message\":\"USB CDC is unavailable in this build\"}");
    return;
  }

  if (enabled && s_implantEnabled && !ECC_ALLOW_CDC_WITH_NCM) {
    controlserver.send(409, "application/json",
                       "{\"ok\":false,\"message\":\"Disable USB Network Implant first: "
                       "the ESP32-S3 cannot enumerate HID + USB Serial + NCM together\"}");
    return;
  }

  if (enabled == s_usbSerialEnabled) {
    controlserver.send(200, "application/json", "{\"ok\":true,\"restarting\":false}");
    return;
  }

  const bool previous = s_usbSerialEnabled;
  s_usbSerialEnabled = enabled;
  if (!eccUsbSerialSaveConfig()) {
    s_usbSerialEnabled = previous;
    controlserver.send(500, "application/json",
                       "{\"ok\":false,\"message\":\"Could not save the setting\"}");
    return;
  }

  controlserver.sendHeader("Cache-Control", "no-store");
  controlserver.sendHeader("Connection", "close");
  controlserver.send(200, "application/json", "{\"ok\":true,\"restarting\":true}");
  delay(500);
  ESP.restart();
}

static void eccUsbSerialRegisterRoutes() {
  controlserver.on("/usb-serial/status", handleUsbSerialStatus);
  controlserver.on("/usb-serial", HTTP_POST, handleUsbSerialSet);
}
