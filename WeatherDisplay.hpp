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
    auto* data = rasterizerClient->getIcon("weather-icons", (String("wi-") + WEATHER_ICON_MAPPINGS[openWeatherClient->getIconId()]).c_str(), 50);
    data->draw(displayManager, 0, 0, WHITE);

    //displayManager.setText(60, 0, "29.5C");
    
    displayManager.rerender();
    delete data;
  }
};
