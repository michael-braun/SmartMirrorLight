#include <math.h>
#include <Fonts/FreeMono12pt7b.h>

class WeatherDisplay {
private:
  DisplayManager displayManager;
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
    auto* data = rasterizerClient->getIcon("weather-icons", (getWeatherIcons(openWeatherClient->getIconId())).c_str(), 60);
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
    
    displayManager.setText(128, 45, (String(tempFull) + "." + String(tempDecimals) + " C").c_str(), &options);
    displayManager.drawCircle(113, 42, 2, WHITE);
    
    displayManager.rerender();
    delete data;
  }
};
