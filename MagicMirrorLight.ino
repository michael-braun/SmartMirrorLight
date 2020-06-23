#include <TM1637Display.h>

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
 
void setup()
{
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
     
  while (WiFi.status() != WL_CONNECTED) {
     
    delay(1000);
    Serial.print("Connecting..");
     
  }

  clockDisplay.setup();

  //weatherDisplay.setup();
  statusDisplay.setup();
  statusDisplay.update();

  openWeatherClient.update();
  //weatherDisplay.update();
}

unsigned int lastUpdate = 0;

void loop()
{
  clockDisplay.loop();
  statusDisplay.loop();

  delay(100);
}
