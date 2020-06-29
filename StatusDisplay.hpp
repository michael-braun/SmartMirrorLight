#include "./U8G2DisplayManager.hpp"
#include "./RasterizerClient.hpp"

class StatusDisplay {
private:
  RasterizerData* data[4] = { NULL, NULL, NULL, NULL };
  int dataLength = 0;

  U8G2DisplayManager displayManager = { U8G2_TYPE_SSD1306, 128, 32 };
  const RasterizerClient* rasterizerClient;

  unsigned long lastUpdate = 0;

  boolean bIsDisabled = false;

public:
  StatusDisplay(const RasterizerClient* rasterizerClient) {
    this->rasterizerClient = rasterizerClient;
  }

  void setup() {
    displayManager.setup();
    displayManager.clear();

    //this->add(rasterizerClient->getIcon("weather-icons", "wi-day-sunny", 32));
    //this->add(rasterizerClient->getIcon("weather-icons", "wi-alien", 32));

    this->update();
  }

  int findIndex(const char* libraryName, const char* iconName) {
    for (int i = 0; i < 4; i++) {
      if (this->data[i] != NULL
        && strcmp(this->data[i]->getLibrary(), libraryName) == 0
        && strcmp(this->data[i]->getIconName(), iconName) == 0) {
        return i;
      }
    }

    return -1;
  }

  bool add(RasterizerData* value) {
    int idx = this->findIndex(value->getLibrary(), value->getIconName());

    if (idx != -1) {
      return false;
    }

    if (dataLength >= 4) {
      return false;
    }
    
    this->data[dataLength] = value;
    dataLength += 1;

    return true;
  }

  bool remove(const char* libraryName, const char* iconName) {
    int idx = this->findIndex(libraryName, iconName);

    if (idx == -1) {
      return false;
    }

    delete this->data[idx];
    this->data[idx] = NULL;

    for (int i = idx; i < 3; i++) {
      RasterizerData* temp = this->data[i];
      this->data[i] = this->data[i + 1];
      this->data[i + 1] = NULL;
    }

    this->dataLength -= 1;

    return true;
  }

  void update() {
    displayManager.updateTCAPort();
    displayManager.clear();
    int x = (128 - (this->dataLength * 32)) / 2;
    for (int i = 0; i < 4; i++) {
      if (data[i] != NULL) {
        data[i]->draw(displayManager, x + (i * 32) + 4, 4, WHITE);
      }
    }
    
    displayManager.rerender();
  }

  void setTCAPort(uint8_t port) {
    this->displayManager.setTCAPort(port);
  }

  void loop() {
    if (this->bIsDisabled || (millis() - this->lastUpdate) < 10000) {
      return;
    }

    Serial.println("loop update status");
    
    HTTPClient http;
    http.begin("http://192.168.178.22:1880/smart-mirror/icons");
    int httpCode = http.GET();

    Serial.println(httpCode);

    if (httpCode == 200) {
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, http.getStream());
      serializeJson(doc, Serial);
      Serial.println("");

      for (int i = 0; i < 4; i++) {
        if (doc[i].isNull()) {
          break;
        }

        if (!doc[i]["library"].is<String>() || !doc[i]["name"].is<String>()) {
          continue;
        }

        String library = doc[i]["library"].as<String>();
        String iconName = doc[i]["name"].as<String>();

        if (this->data[i] != NULL) {
          if (strcmp(this->data[i]->getLibrary(), library.c_str()) == 0
            && strcmp(this->data[i]->getIconName(), iconName.c_str()) == 0) {
            continue;
          } else {
            this->remove(this->data[i]->getLibrary(), this->data[i]->getIconName());
          }
        }

        int idx = this->findIndex(library.c_str(), iconName.c_str());
        if (idx != -1) {
          continue;
        }

        serializeJson(doc[i], Serial);

        auto* rasterizerData = this->rasterizerClient->getIcon(library.c_str(), iconName.c_str(), 24);
        this->add(rasterizerData);
      }
      
      this->update();
    }
    
    //Serial.println(payload);
    http.end();

    this->lastUpdate = millis();
  }

  void enable() {
    this->bIsDisabled = false;
    this->update();
  }

  void disable() {
    this->bIsDisabled = true;
    this->displayManager.clear();
    this->displayManager.rerender();
    Serial.println("disable");
  }

  ~StatusDisplay() {
    for (int i = 0; i < 4; i++) {
      if (data[i] != NULL) {
        delete data[i];
      }
    }
  }
};
