#include <math.h>
#include ".\U8G2DisplayManager.hpp"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMono12pt7b.h>

class WeatherDisplay {
private:
  U8G2DisplayManager displayManager = { U8G2_TYPE_SH1106, 128, 64 };
  const OpenWeatherClient* openWeatherClient;
  const RasterizerClient* rasterizerClient;

public:
  WeatherDisplay(const OpenWeatherClient* openWeatherClient, const RasterizerClient* rasterizerClient) {
    this->openWeatherClient = openWeatherClient;
    this->rasterizerClient = rasterizerClient;
  }

  void setup() {
    displayManager.setup();
    displayManager.clear();
  }

  void update() {
    this->displayManager.clear();
    
    auto* data = rasterizerClient->getIcon("weather-icons", (getWeatherIcons(openWeatherClient->getIconId())).c_str(), 48);
    data->draw(displayManager, 0, 0, WHITE);

    FontOptions options {
      &FreeMono12pt7b,
      2,
      WHITE,
      TEXT_ALIGN_RIGHT
    };

    int temp = openWeatherClient->getTemp();
    int tempFull = (int) temp;
    int tempDecimals = round((tempFull * 10) - (temp * 10));
    
    displayManager.setText(128, 64, (String(tempFull) + "." + String(tempDecimals) + " C").c_str(), &options);
    displayManager.drawCircle(112, 42, 2, WHITE);
    displayManager.drawCircle(112, 42, 3, WHITE);

    displayManager.rerender();

    delete data;
  }

  void enable() {
    this->update();
    this->displayManager.enable();
  }

  void disable() {
    this->displayManager.clear();
    this->displayManager.disable();
  }

  void setTCAPort(uint8_t port) {
    this->displayManager.setTCAPort(port);
  }

  void loop() {
  }
};
