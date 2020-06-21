#include <TM1637Display.h>

#include "./Configuration.hpp"
#include "./DigitDisplayManager.hpp"
#include "./DisplayManager.hpp"
#include "./RasterizerClient.hpp"
#include "./OpenWeatherClient.hpp"
#include "./WeatherDisplay.hpp"
#include "./ClockHelper.hpp"

 
const int CLK = D6; //Set the CLK pin connection to the display
const int DIO = D5; //Set the DIO pin connection to the display
 
int lastClock = 0;
 
DigitDisplayManager digitDisplayManager(CLK, DIO); //set up the 4-Digit Display.
OpenWeatherClient openWeatherClient = { OPEN_WEATHER_APP_ID, OPEN_WEATHER_ZIP };
RasterizerClient rasterizerClient = { RASTERIZR_SERVER };
WeatherDisplay weatherDisplay = { &openWeatherClient, &rasterizerClient };
ClockHelper clockHelper;
 
void setup()
{
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
     
  while (WiFi.status() != WL_CONNECTED) {
     
    delay(1000);
    Serial.print("Connecting..");
     
  }

  clockHelper.setup();

  digitDisplayManager.setOrientation(ORIENTATION_INVERSE);
  digitDisplayManager.setDots(true);

  digitDisplayManager.setTransition(500);
  digitDisplayManager.setBrightness(0);
  delay(1000);

  weatherDisplay.setup();

  openWeatherClient.update();
  weatherDisplay.update();
}

void loop()
{
  clockHelper.loop();
  digitDisplayManager.loop();

  //displayManager.animate();
  if (!digitDisplayManager.isAnimating()) {
    int epoch = clockHelper.getEpochTime();
    int hours = ((epoch % 86400L) / 3600);
    int minutes = ((epoch % 3600) / 60);

    int newClock = hours * 100 + minutes;

    if (newClock != lastClock) {
      digitDisplayManager.showNumberDec(newClock, true); //Display the numCounter value;
      digitDisplayManager.animate();

      lastClock = newClock;
    }
  }

  delay(10000);
}
