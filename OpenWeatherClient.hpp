#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <unordered_map>

std::unordered_map<int, const char*> WEATHER_ICON_MAPPINGS = {
    { 200, "storm-showers" },
    { 201, "storm-showers" },
    { 202, "storm-showers" },
    { 210, "storm-showers" },
    { 211, "thunderstorm" },
    { 212, "thunderstorm" },
    { 221, "thunderstorm" },
    { 230, "storm-showers" },
    { 231, "storm-showers" },
    { 232, "storm-showers" },
    { 300, "sprinkle" },
    { 301, "sprinkle" },
    { 302, "sprinkle" },
    { 310, "sprinkle" },
    { 311, "sprinkle" },
    { 312, "sprinkle" },
    { 313, "sprinkle" },
    { 314, "sprinkle" },
    { 321, "sprinkle" },
    { 500, "rain" },
    { 501, "rain" },
    { 502, "rain" },
    { 503, "rain" },
    { 504, "rain" },
    { 511, "rain-mix" },
    { 520, "showers" },
    { 521, "showers" },
    { 522, "showers" },
    { 531, "showers" },
    { 600, "snow" },
    { 601, "snow" },
    { 602, "snow" },
    { 611, "sleet" },
    { 612, "sleet" },
    { 615, "rain-mix" },
    { 616, "rain-mix" },
    { 620, "rain-mix" },
    { 621, "rain-mix" },
    { 622, "rain-mix" },
    { 701, "sprinkle" },
    { 711, "smoke" },
    { 721, "day-haze" },
    { 731, "cloudy-gusts" },
    { 741, "fog" },
    { 751, "cloudy-gusts" },
    { 761, "dust" },
    { 762, "smog" },
    { 771, "day-windy" },
    { 781, "tornado" },
    { 800, "sunny" },
    { 801, "cloudy" },
    { 802, "cloudy" },
    { 803, "cloudy" },
    { 804, "cloudy" },
    { 900, "tornado" },
    { 901, "hurricane" },
    { 902, "hurricane" },
    { 903, "snowflake-cold" },
    { 904, "hot" },
    { 905, "windy" },
    { 906, "hail" },
    { 951, "sunny" },
    { 952, "cloudy-gusts" },
    { 953, "cloudy-gusts" },
    { 954, "cloudy-gusts" },
    { 955, "cloudy-gusts" },
    { 956, "cloudy-gusts" },
    { 957, "cloudy-gusts" },
    { 958, "cloudy-gusts" },
    { 959, "cloudy-gusts" },
    { 960, "thunderstorm" },
    { 961, "thunderstorm" },
    { 962, "cloudy-gusts" }
};

class OpenWeatherClient {
private:
  String appid;
  String zip;
  int iconId;
  float temp;
  RasterizerData* iconData;
  
public:
  OpenWeatherClient(String appid, String zip) {
    this->appid = appid;
    this->zip = zip;
  }

  void update() {
      HTTPClient http;
      http.begin("http://api.openweathermap.org/data/2.5/forecast?zip=" + this->zip + "&appid=" + this->appid);
      int httpCode = http.GET();

      Serial.println(httpCode);

      DynamicJsonDocument doc(2048);
      deserializeJson(doc, http.getStream());

      temp = doc["list"][1]["main"]["temp"].as<float>() / 10;

      this->iconId = doc["list"][1]["weather"][0]["id"].as<int>();

      serializeJson(doc["list"][1], Serial);
      serializeJson(doc["list"][1]["weather"][0], Serial);
      
      //Serial.println(payload);
      http.end();
  }

  int getIconId() const {
    return this->iconId;
  }

  float getTemp() const {
    return this->temp;
  }
};
