#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

extern WebServer controlserver;

static const char* ECC_SAPSAN_HOST         = "https://sapsan-terminal.com";
static const char* ECC_SAPSAN_SESSION_PATH = "/sapsan_session.txt";
static const char* ECC_SAPSAN_TARGET_DEVICE = "evil_crow_cable";

#ifndef ECC_SAPSAN_ROOT_CA
#define ECC_SAPSAN_ROOT_CA nullptr
#endif

static const char* const ECC_SAPSAN_CA = ECC_SAPSAN_ROOT_CA;

#ifndef ECC_SAPSAN_MIN_HEAP
#define ECC_SAPSAN_MIN_HEAP 48000U
#endif

static String s_sapsanSession;
static String s_sapsanName;
static String s_sapsanEmail;
static String s_sapsanTier;
static String s_sapsanStatusError;

static SemaphoreHandle_t s_sapsanNetMutex = nullptr;
static SemaphoreHandle_t s_sapsanJobMutex = nullptr;

enum EccSapsanJobState : uint8_t {
  ECC_SAPSAN_IDLE = 0,
  ECC_SAPSAN_RUNNING,
  ECC_SAPSAN_DONE,
  ECC_SAPSAN_FAILED
};

static volatile EccSapsanJobState s_sapsanJobState = ECC_SAPSAN_IDLE;
static String s_sapsanJobPrompt;
static String s_sapsanJobEditor;
static String s_sapsanJobCode;
static String s_sapsanJobRaw;
static String s_sapsanJobModel;
static String s_sapsanJobError;
static TaskHandle_t s_sapsanTaskHandle = nullptr;

static void eccSapsanEnsureMutexes() {
  if (!s_sapsanNetMutex) s_sapsanNetMutex = xSemaphoreCreateMutex();
  if (!s_sapsanJobMutex) s_sapsanJobMutex = xSemaphoreCreateMutex();
}

static String eccSapsanJsonEscape(const String& in) {
  String out;
  out.reserve(in.length() + 16);
  for (size_t i = 0; i < in.length(); ++i) {
    const char c = in[i];
    switch (c) {
      case '"':  out += "\\\""; break;
      case '\\': out += "\\\\"; break;
      case '\n': out += "\\n";  break;
      case '\r': out += "\\r";  break;
      case '\t': out += "\\t";  break;
      default:
        if ((uint8_t)c < 0x20) { char b[8]; snprintf(b, sizeof(b), "\\u%04x", c); out += b; }
        else out += c;
    }
  }
  return out;
}

static void eccSapsanLoadSession() {
  s_sapsanSession = "";
  s_sapsanEmail = "";
  if (!LittleFS.exists(ECC_SAPSAN_SESSION_PATH)) return;
  File f = LittleFS.open(ECC_SAPSAN_SESSION_PATH, FILE_READ);
  if (!f) return;
  s_sapsanSession = f.readStringUntil('\n'); s_sapsanSession.trim();
  s_sapsanEmail   = f.readStringUntil('\n'); s_sapsanEmail.trim();
  f.close();
}

static bool eccSapsanSaveSession() {
  File f = LittleFS.open(ECC_SAPSAN_SESSION_PATH, FILE_WRITE);
  if (!f) return false;
  f.println(s_sapsanSession);
  f.println(s_sapsanEmail);
  f.flush();
  f.close();
  return true;
}

static void eccSapsanClearSession() {
  s_sapsanSession = "";
  s_sapsanName = "";
  s_sapsanTier = "";
  if (LittleFS.exists(ECC_SAPSAN_SESSION_PATH)) LittleFS.remove(ECC_SAPSAN_SESSION_PATH);
}

static bool eccSapsanPrepare(WiFiClientSecure& client, HTTPClient& https,
                             const String& url, const char* referer, String& err) {
  if (WiFi.status() != WL_CONNECTED) { err = "Wi-Fi is not connected"; return false; }
  if (ESP.getFreeHeap() < ECC_SAPSAN_MIN_HEAP) {
    err = "Not enough free memory for a TLS connection (" + String(ESP.getFreeHeap()) + " bytes)";
    return false;
  }

  if (ECC_SAPSAN_CA) client.setCACert(ECC_SAPSAN_CA);
  else               client.setInsecure();
  client.setTimeout(30);
  https.setReuse(false);
  https.setConnectTimeout(15000);
  https.setTimeout(60000);

  https.setUserAgent("Mozilla/5.0 (X11; Linux x86_64; rv:128.0) Gecko/20100101 Firefox/128.0");
  if (!https.begin(client, url)) { err = "Could not open the HTTPS connection"; return false; }

  https.addHeader("Content-Type", "application/json");
  https.addHeader("Accept", "*/*");
  https.addHeader("Accept-Encoding", "identity");
  https.addHeader("Origin", ECC_SAPSAN_HOST);
  https.addHeader("Referer", referer);
  if (s_sapsanSession.length()) {
    https.addHeader("Cookie", "app_session_id=" + s_sapsanSession);
  }
  return true;
}

static String eccSapsanTrpcError(JsonDocument& doc) {
  JsonVariant e = doc[0]["error"]["json"]["message"];
  if (!e.isNull()) return e.as<String>();
  e = doc[0]["error"]["message"];
  if (!e.isNull()) return e.as<String>();
  return String();
}

static bool eccSapsanLogin(const String& email, const String& password, String& err) {
  eccSapsanEnsureMutexes();
  if (xSemaphoreTake(s_sapsanNetMutex, pdMS_TO_TICKS(30000)) != pdTRUE) {
    err = "Another SAPSAN request is still running"; return false;
  }

  bool ok = false;
  {
    WiFiClientSecure client;
    HTTPClient https;
    const String url = String(ECC_SAPSAN_HOST) + "/api/trpc/auth.loginEmail?batch=1";

    if (eccSapsanPrepare(client, https, url, "https://sapsan-terminal.com/", err)) {
      const char* collect[] = { "Set-Cookie" };
      https.collectHeaders(collect, 1);

      String body = "{\"0\":{\"json\":{\"email\":\"" + eccSapsanJsonEscape(email) +
                    "\",\"password\":\"" + eccSapsanJsonEscape(password) + "\"}}}";

      const int code = https.POST(body);
      body = "";

      if (code <= 0) {
        err = "Connection failed: " + https.errorToString(code);
      } else {
        String payload = https.getString();
        JsonDocument doc;
        DeserializationError jerr = deserializeJson(doc, payload);

        if (code != 200) {
          String msg = jerr ? String() : eccSapsanTrpcError(doc);
          err = msg.length() ? msg : ("Login rejected (HTTP " + String(code) + ")");
        } else if (jerr) {
          err = "Malformed server response";
        } else {
          const bool success = doc[0]["result"]["data"]["json"]["success"] | false;
          String cookie = https.header("Set-Cookie");
          int p = cookie.indexOf("app_session_id=");
          if (!success) {
            String msg = eccSapsanTrpcError(doc);
            err = msg.length() ? msg : "Invalid email or password";
          } else if (p < 0) {
            err = "The server did not return a session cookie";
          } else {
            p += 15;
            int e = cookie.indexOf(';', p);
            s_sapsanSession = (e < 0) ? cookie.substring(p) : cookie.substring(p, e);
            s_sapsanSession.trim();
            s_sapsanEmail = email;
            if (!eccSapsanSaveSession()) err = "Logged in but the session could not be saved";
            ok = true;
          }
        }
      }
      https.end();
    }
    client.stop();
  }

  xSemaphoreGive(s_sapsanNetMutex);
  return ok;
}

static bool eccSapsanFetchMe(String& err) {
  if (!s_sapsanSession.length()) { err = "Not logged in"; return false; }

  eccSapsanEnsureMutexes();
  if (xSemaphoreTake(s_sapsanNetMutex, pdMS_TO_TICKS(30000)) != pdTRUE) {
    err = "Another SAPSAN request is still running"; return false;
  }

  bool ok = false;
  {
    WiFiClientSecure client;
    HTTPClient https;

    const String url = String(ECC_SAPSAN_HOST) +
      "/api/trpc/auth.me?batch=1&input=%7B%220%22%3A%7B%22json%22%3Anull%2C%22meta%22%3A%7B%22values%22%3A%5B%22undefined%22%5D%7D%7D%7D";

    if (eccSapsanPrepare(client, https, url, "https://sapsan-terminal.com/", err)) {
      const int code = https.GET();
      if (code <= 0) {
        err = "Connection failed: " + https.errorToString(code);
      } else if (code == 401 || code == 403) {
        err = "The session expired, please log in again";
        eccSapsanClearSession();
      } else {
        String payload = https.getString();
        JsonDocument doc;
        JsonDocument filter;
        filter[0]["result"]["data"]["json"]["name"]             = true;
        filter[0]["result"]["data"]["json"]["email"]            = true;
        filter[0]["result"]["data"]["json"]["subscriptionTier"] = true;
        filter[0]["error"] = true;

        if (deserializeJson(doc, payload, DeserializationOption::Filter(filter))) {
          err = "Malformed server response";
        } else {
          JsonVariant u = doc[0]["result"]["data"]["json"];
          if (u.isNull()) {
            String msg = eccSapsanTrpcError(doc);
            err = msg.length() ? msg : "The session is no longer valid";
            eccSapsanClearSession();
          } else {
            s_sapsanName  = u["name"].as<String>();
            s_sapsanEmail = u["email"].as<String>();
            s_sapsanTier  = u["subscriptionTier"].as<String>();
            ok = true;
          }
        }
      }
      https.end();
    }
    client.stop();
  }

  xSemaphoreGive(s_sapsanNetMutex);
  return ok;
}

static String eccSapsanExtractWindCode(const String& md) {
  int start = -1;
  int p = md.indexOf("```wind");
  if (p >= 0) {
    start = p + 7;
  } else {
    p = md.indexOf("```");
    if (p >= 0) {
      const int nl = md.indexOf('\n', p + 3);
      start = (nl >= 0) ? nl : p + 3;
    }
  }
  if (start < 0) {
    String raw = md; raw.trim(); return raw;
  }
  if (start < (int)md.length() && md.charAt(start) == '\r') start++;
  if (start < (int)md.length() && md.charAt(start) == '\n') start++;

  int end = md.indexOf("```", start);
  if (end < 0) end = md.length();
  String code = md.substring(start, end);
  code.trim();
  return code;
}

static void eccSapsanAskTask(void* arg) {
  (void)arg;
  String err, code, raw, model;
  bool ok = false;

  String prompt, editor;
  if (xSemaphoreTake(s_sapsanJobMutex, portMAX_DELAY) == pdTRUE) {
    prompt = s_sapsanJobPrompt;
    editor = s_sapsanJobEditor;
    xSemaphoreGive(s_sapsanJobMutex);
  }

  if (!s_sapsanSession.length()) {
    err = "Not logged in to SAPSAN TERMINAL";
  } else if (xSemaphoreTake(s_sapsanNetMutex, pdMS_TO_TICKS(60000)) != pdTRUE) {
    err = "Another SAPSAN request is still running";
  } else {
    {
      WiFiClientSecure client;
      HTTPClient https;
      const String url = String(ECC_SAPSAN_HOST) + "/api/trpc/ai.chat?batch=1";

      if (eccSapsanPrepare(client, https, url, "https://sapsan-terminal.com/editor", err)) {
        const bool haveEditor = editor.length() > 0;
        String body = "{\"0\":{\"json\":{\"message\":\"" + eccSapsanJsonEscape(prompt) + "\"";
        body += ",\"editorCode\":";
        body += haveEditor ? ("\"" + eccSapsanJsonEscape(editor) + "\"") : String("null");
        body += ",\"history\":[],\"targetDevice\":\"" + String(ECC_SAPSAN_TARGET_DEVICE) + "\"";
        body += ",\"targetOS\":null,\"model\":null,\"useFastModel\":false}";
        body += ",\"meta\":{\"values\":{";
        if (!haveEditor) body += "\"editorCode\":[\"undefined\"],";
        body += "\"targetOS\":[\"undefined\"],\"model\":[\"undefined\"]}}}}";

        const int rc = https.POST(body);
        body = "";

        if (rc <= 0) {
          err = "Connection failed: " + https.errorToString(rc);
        } else if (rc == 401 || rc == 403) {
          err = "The session expired, please log in again";
          eccSapsanClearSession();
        } else {
          String payload = https.getString();
          JsonDocument doc;
          JsonDocument filter;
          filter[0]["result"]["data"]["json"]["response"] = true;
          filter[0]["result"]["data"]["json"]["model"]    = true;
          filter[0]["error"] = true;

          if (deserializeJson(doc, payload, DeserializationOption::Filter(filter))) {
            err = "Malformed server response";
          } else {
            JsonVariant r = doc[0]["result"]["data"]["json"]["response"];
            if (r.isNull()) {
              String msg = eccSapsanTrpcError(doc);
              err = msg.length() ? msg : ("The assistant returned no answer (HTTP " + String(rc) + ")");
            } else {
              raw   = r.as<String>();
              model = doc[0]["result"]["data"]["json"]["model"].as<String>();
              code  = eccSapsanExtractWindCode(raw);
              ok    = code.length() > 0;
              if (!ok) err = "The answer contained no payload code";
            }
          }
        }
        https.end();
      }
      client.stop();
    }
    xSemaphoreGive(s_sapsanNetMutex);
  }

  if (xSemaphoreTake(s_sapsanJobMutex, portMAX_DELAY) == pdTRUE) {
    s_sapsanJobCode  = code;
    s_sapsanJobRaw   = raw;
    s_sapsanJobModel = model;
    s_sapsanJobError = err;
    s_sapsanJobPrompt = "";
    s_sapsanJobEditor = "";
    s_sapsanJobState = ok ? ECC_SAPSAN_DONE : ECC_SAPSAN_FAILED;
    xSemaphoreGive(s_sapsanJobMutex);
  }
  s_sapsanTaskHandle = nullptr;
  vTaskDelete(nullptr);
}

static void eccSapsanHandleStatus() {
  String json = "{";
  json += "\"loggedIn\":" + String(s_sapsanSession.length() ? "true" : "false");
  json += ",\"email\":\"" + eccSapsanJsonEscape(s_sapsanEmail) + "\"";
  json += ",\"name\":\"" + eccSapsanJsonEscape(s_sapsanName) + "\"";
  json += ",\"tier\":\"" + eccSapsanJsonEscape(s_sapsanTier) + "\"";
  json += ",\"wifi\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false");
  json += ",\"busy\":" + String(s_sapsanJobState == ECC_SAPSAN_RUNNING ? "true" : "false");
  json += ",\"error\":\"" + eccSapsanJsonEscape(s_sapsanStatusError) + "\"";
  json += "}";
  controlserver.sendHeader("Cache-Control", "no-store");
  controlserver.send(200, "application/json", json);
}

static void eccSapsanHandleLogin() {
  if (!controlserver.hasArg("email") || !controlserver.hasArg("password")) {
    controlserver.send(400, "application/json",
                       "{\"ok\":false,\"message\":\"Missing email or password\"}");
    return;
  }
  String email = controlserver.arg("email");
  String password = controlserver.arg("password");
  email.trim();

  String err;
  const bool ok = eccSapsanLogin(email, password, err);
  password = "";
  s_sapsanStatusError = ok ? String() : err;

  if (ok) {
    String meErr;
    eccSapsanFetchMe(meErr);
  }

  String json = "{\"ok\":";
  json += ok ? "true" : "false";
  json += ",\"name\":\"" + eccSapsanJsonEscape(s_sapsanName) + "\"";
  json += ",\"tier\":\"" + eccSapsanJsonEscape(s_sapsanTier) + "\"";
  json += ",\"message\":\"" + eccSapsanJsonEscape(ok ? String("Logged in") : err) + "\"}";
  controlserver.sendHeader("Cache-Control", "no-store");
  controlserver.send(ok ? 200 : 401, "application/json", json);
}

static void eccSapsanHandleLogout() {
  eccSapsanClearSession();
  s_sapsanStatusError = "";
  controlserver.sendHeader("Cache-Control", "no-store");
  controlserver.send(200, "application/json", "{\"ok\":true}");
}

static void eccSapsanHandleAsk() {
  eccSapsanEnsureMutexes();

  if (!controlserver.hasArg("prompt")) {
    controlserver.send(400, "application/json",
                       "{\"ok\":false,\"message\":\"Missing prompt\"}");
    return;
  }
  if (!s_sapsanSession.length()) {
    controlserver.send(401, "application/json",
                       "{\"ok\":false,\"message\":\"Log in to SAPSAN TERMINAL in Config first\"}");
    return;
  }
  if (s_sapsanJobState == ECC_SAPSAN_RUNNING) {
    controlserver.send(409, "application/json",
                       "{\"ok\":false,\"message\":\"A request is already running\"}");
    return;
  }

  if (xSemaphoreTake(s_sapsanJobMutex, portMAX_DELAY) == pdTRUE) {
    s_sapsanJobPrompt = controlserver.arg("prompt");
    s_sapsanJobEditor = controlserver.hasArg("editorCode") ? controlserver.arg("editorCode") : String();
    s_sapsanJobCode = ""; s_sapsanJobRaw = ""; s_sapsanJobModel = ""; s_sapsanJobError = "";
    s_sapsanJobState = ECC_SAPSAN_RUNNING;
    xSemaphoreGive(s_sapsanJobMutex);
  }

  if (xTaskCreatePinnedToCore(eccSapsanAskTask, "sapsan_ai", 12288, nullptr,
                              1, &s_sapsanTaskHandle, 1) != pdPASS) {
    s_sapsanJobState = ECC_SAPSAN_FAILED;
    s_sapsanJobError = "Could not start the request task";
    controlserver.send(500, "application/json",
                       "{\"ok\":false,\"message\":\"Could not start the request task\"}");
    return;
  }

  controlserver.sendHeader("Cache-Control", "no-store");
  controlserver.send(202, "application/json", "{\"ok\":true,\"pending\":true}");
}

static void eccSapsanHandleAskResult() {
  String json = "{";
  const EccSapsanJobState st = s_sapsanJobState;

  if (st == ECC_SAPSAN_RUNNING) {
    json += "\"state\":\"running\"}";
  } else if (st == ECC_SAPSAN_IDLE) {
    json += "\"state\":\"idle\"}";
  } else if (xSemaphoreTake(s_sapsanJobMutex, pdMS_TO_TICKS(2000)) == pdTRUE) {
    json += "\"state\":\"";
    json += (st == ECC_SAPSAN_DONE) ? "done" : "failed";
    json += "\",\"code\":\"" + eccSapsanJsonEscape(s_sapsanJobCode) + "\"";
    json += ",\"model\":\"" + eccSapsanJsonEscape(s_sapsanJobModel) + "\"";
    json += ",\"message\":\"" + eccSapsanJsonEscape(s_sapsanJobError) + "\"}";
    xSemaphoreGive(s_sapsanJobMutex);
  } else {
    json += "\"state\":\"running\"}";
  }

  controlserver.sendHeader("Cache-Control", "no-store");
  controlserver.send(200, "application/json", json);
}

static void eccSapsanBegin() {
  eccSapsanEnsureMutexes();
  eccSapsanLoadSession();
}

static void eccSapsanRegisterRoutes() {
  controlserver.on("/sapsan/status", eccSapsanHandleStatus);
  controlserver.on("/sapsan/login",  HTTP_POST, eccSapsanHandleLogin);
  controlserver.on("/sapsan/logout", HTTP_POST, eccSapsanHandleLogout);
  controlserver.on("/sapsan/ask",    HTTP_POST, eccSapsanHandleAsk);
  controlserver.on("/sapsan/ask/result", eccSapsanHandleAskResult);
}
