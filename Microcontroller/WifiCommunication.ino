#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "<2G WiFi network name>";
const char* password = "<WiFi Password>";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
ESP8266WebServer server(80);

char* nextUpdateTime = "";

void handleRoot() {
  JsonDocument doc;
  doc["mode"] = currentModeToString();
  doc["thermostatSetting"] = acIsOnToString();
  doc["lastUpdateTime"] = timeClient.getFormattedTime();
  String jsonOutput;
  serializeJson(doc, jsonOutput);
  server.send(200, "application/json", jsonOutput);
}

void handleSetSchedule() {
  currentMode = scheduled;
  flashLED();
  if (server.hasArg("restTime")) {
    acRestTime = server.arg("restTime");
  }
  turnACOnWithSchedule();
}

void handleTurnAcOn() {
  currentMode = onUntilStopped;
  flashLED();
  turnACOn();
  handleRoot();
}

void handleTurnAcOff() {
  currentMode = onUntilStopped;
  flashLED();
  turnACOff();
  handleRoot();
}

void setupWifi() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.hostname("Chucks ESP8266");

  while (WiFi.status() != WL_CONNECTED) {
    toggleLED();
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi.");
  flashLED();
  flashLED();
  server.on("/", handleRoot);
  server.on("/turnOn", handleTurnAcOn);
  server.on("/turnOff", handleTurnAcOff);
  server.on("/setSchedule", handleSetSchedule);
  server.begin();
  Serial.print("Listening at ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
}

const unsigned long networkUpdateInterval = 500;
unsigned long nextNetworkUpdateTime = 0;
void listenForNetworkUpdates(unsigned long currentClockTime) {
  if (currentClockTime > nextNetworkUpdateTime) {
    server.handleClient();
    nextNetworkUpdateTime = currentClockTime + networkUpdateInterval;
  }
}

const unsigned long ntpSynchronizationInterval = 60000;
unsigned long nextNTPUpdateTime = 0;
void updateNTPTimeIfNecessary(unsigned long currentClockTime) {
  if (currentClockTime > nextNTPUpdateTime) {
    timeClient.update();
    Serial.println(timeClient.getFormattedTime());
    nextNTPUpdateTime = currentClockTime + ntpSynchronizationInterval;
  }
}