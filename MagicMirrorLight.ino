#include <TM1637Display.h>
#include <SparkFun_APDS9960.h>

#include "./GestureManager.hpp"

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
WeatherDisplay weatherDisplay = { &openWeatherClient, &rasterizerClient };
ClockDisplay clockDisplay;
//GestureManager gestureManager;
StatusDisplay statusDisplay = { &rasterizerClient };

DynamicJsonDocument doc(2048);

ESP8266WebServer server(80);

SparkFun_APDS9960 apds = SparkFun_APDS9960();

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

void handleLeft() {
  Serial.println("LEFT");
}
 
void setup()
{
  Serial.begin(9600);

  Wire.begin(D3, D4);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
     
  while (WiFi.status() != WL_CONNECTED) {
     
    delay(1000);
    Serial.print("Connecting..");
     
  }

  server.on("/", HTTPMethod::HTTP_POST, handleAdd);
  server.on("/", HTTPMethod::HTTP_DELETE, handleRemove);

  clockDisplay.setup();

  openWeatherClient.update();

  statusDisplay.setTCAPort(0);
  statusDisplay.setup();
  statusDisplay.update();

  weatherDisplay.setTCAPort(1);
  weatherDisplay.setup();
  weatherDisplay.update();

  server.begin();

  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
}

unsigned int lastUpdate = 0;

void loop()
{
  if ( apds.isGestureAvailable() ) {
      Serial.println("gesture available");
      Serial.println(apds.readGesture());
      }
  
  Serial.println("loop 1");
  server.handleClient();
  Serial.println("loop 2");
  clockDisplay.loop();
  Serial.println("loop 3");
  statusDisplay.loop();
  Serial.println("loop 4");
  weatherDisplay.loop();
  Serial.println("loop 5");

  delay(100);
}
