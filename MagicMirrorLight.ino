#include <TM1637Display.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include "./Configuration.hpp"
#include "./DigitDisplayManager.hpp"
#include "./DisplayManager.hpp"
#include "./RasterizerClient.hpp"
#include "./OpenWeatherClient.hpp"
#include "./WeatherDisplay.hpp"
#include "./ClockDisplay.hpp"
#include "./StatusDisplay.hpp"


OpenWeatherClient openWeatherClient = { OPEN_WEATHER_APP_ID, OPEN_WEATHER_ZIP };
RasterizerClient rasterizerClient = { RASTERIZR_SERVER };
//WeatherDisplay weatherDisplay = { &openWeatherClient, &rasterizerClient };
ClockDisplay clockDisplay;
StatusDisplay statusDisplay = { &rasterizerClient };

DynamicJsonDocument doc(2048);

ESP8266WebServer server(80);

void handleAdd() {
  if (server.hasArg("plain") == false) {
    server.send(400, "text/plain", "");
    return;
  }
  
  deserializeJson(doc, server.arg("plain"));

  const char* icon = doc["icon"].as<char*>();
  const char* library = doc["library"].as<char*>();

  auto* rasterizerData = rasterizerClient.getIcon(library, icon, 32);

  if (!statusDisplay.add(rasterizerData)) {
    server.send(409, "text/plain", "");
    return;
  }

  statusDisplay.update();
  server.send(200, "text/plain", "");
}

void handleRemove() {
  if (server.hasArg("plain") == false) {
    server.send(400, "text/plain", "");
    return;
  }
  
  deserializeJson(doc, server.arg("plain"));

  const char* icon = doc["icon"].as<char*>();
  const char* library = doc["library"].as<char*>();

  if (!statusDisplay.remove(library, icon)) {
    server.send(409, "text/plain", "");
    return;
  }

  statusDisplay.update();
  server.send(200, "text/plain", "");
}
 
void setup()
{
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
     
  while (WiFi.status() != WL_CONNECTED) {
     
    delay(1000);
    Serial.print("Connecting..");
     
  }

  server.on("/", HTTPMethod::HTTP_POST, handleAdd);
  server.on("/", HTTPMethod::HTTP_DELETE, handleRemove);

  clockDisplay.setup();

  //weatherDisplay.setup();
  statusDisplay.setup();
  statusDisplay.update();

  openWeatherClient.update();
  //weatherDisplay.update();

  server.begin();
}

unsigned int lastUpdate = 0;

void loop()
{
  server.handleClient();
  clockDisplay.loop();
  statusDisplay.loop();

  delay(100);
}
