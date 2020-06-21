#include <TM1637Display.h>
#include "./DigitDisplayManager.hpp"
#include "./DisplayManager.hpp"
#include "./RasterizerClient.hpp"
#include "./OpenWeatherClient.hpp"
#include "./WeatherDisplay.hpp"
#include "./Configuration.hpp"
 
const int CLK = D6; //Set the CLK pin connection to the display
const int DIO = D5; //Set the DIO pin connection to the display
 
int numCounter = 0;
 
DigitDisplayManager digitDisplayManager(CLK, DIO); //set up the 4-Digit Display.
OpenWeatherClient openWeatherClient = { OPEN_WEATHER_APP_ID, OPEN_WEATHER_ZIP };
RasterizerClient rasterizerClient = { RASTERIZR_SERVER };
WeatherDisplay weatherDisplay = { &openWeatherClient, &rasterizerClient };
 
void setup()
{
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
     
  while (WiFi.status() != WL_CONNECTED) {
     
    delay(1000);
    Serial.print("Connecting..");
     
  }

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
  digitDisplayManager.loop();

  //displayManager.animate();
  if (!digitDisplayManager.isAnimating()) {
    digitDisplayManager.showNumberDec(numCounter, true); //Display the numCounter value;
    
    delay(1000);

    numCounter++;
  }
}
