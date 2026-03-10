/*
  ESP8266 Sensor Node — sends JSON to online backend
  Replace YOUR_BACKEND_URL with your deployed Render/Railway/etc URL
  
  Required libraries (install via Arduino Library Manager):
    - ArduinoJson by Benoit Blanchon
    - ESP8266WiFi (comes with ESP8266 board package)
    - ESP8266HTTPClient (comes with ESP8266 board package)
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// ── WiFi credentials ──────────────────────────────────────────
const char* ssid     = "mayur";
const char* password = "12345678";

// ── Backend URL ───────────────────────────────────────────────
// After deploying your backend, paste the URL here:
const char* serverUrl = "https://YOUR_BACKEND_URL/api/data";

// ── Sensor variables (populated by your existing sensor code) ─
float temp = 0, hum = 0, mq4 = 0, mq7 = 0;
float co2 = 0, dist = 0, flow = 0;
String metal = "NONE";

// ── How often to send data (milliseconds) ────────────────────
const unsigned long SEND_INTERVAL = 3000;
unsigned long lastSend = 0;

void setup() {
  Serial.begin(9600);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected! IP: " + WiFi.localIP().toString());
}

void loop() {
  // ── READ YOUR SENSORS HERE ──────────────────────────────────
  // Replace these with your actual sensor reading code:
  // temp = dht.readTemperature();
  // hum  = dht.readHumidity();
  // mq4  = analogRead(A0);
  // etc.
  // ───────────────────────────────────────────────────────────

  unsigned long now = millis();
  if (now - lastSend >= SEND_INTERVAL) {
    lastSend = now;
    sendData();
  }
}

void sendData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WARN] WiFi disconnected, skipping send.");
    return;
  }

  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(8000);

  // Build JSON payload
  StaticJsonDocument<256> doc;
  doc["temp"]  = temp;
  doc["hum"]   = hum;
  doc["mq4"]   = mq4;
  doc["mq7"]   = mq7;
  doc["co2"]   = co2;
  doc["dist"]  = dist;
  doc["flow"]  = flow;
  doc["metal"] = metal;

  String body;
  serializeJson(doc, body);

  Serial.println("[SEND] " + body);
  int httpCode = http.POST(body);

  if (httpCode > 0) {
    Serial.println("[OK] HTTP " + String(httpCode));
  } else {
    Serial.println("[ERR] " + http.errorToString(httpCode));
  }

  http.end();
}
