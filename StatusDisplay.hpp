#include "./RasterizerClient.hpp"

class StatusDisplay {
private:
  RasterizerData* data[4] = { NULL, NULL, NULL, NULL };
  int dataLength = 0;

  DisplayManager displayManager = { 128, 32 };
  const RasterizerClient* rasterizerClient;

public:
  StatusDisplay(const RasterizerClient* rasterizerClient) {
    this->rasterizerClient = rasterizerClient;
  }

  void setup() {
    displayManager.setup();
    displayManager.clear();

    auto* sunny = rasterizerClient->getIcon("weather-icons", "wi-day-sunny", 32);
    this->add(sunny);

    auto* rain = rasterizerClient->getIcon("weather-icons", "wi-rain", 32);
    this->add(rain);

    this->update();

    delay(1000);

    this->remove("weather-icons", "wi-day-sunny");
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

  void add(RasterizerData* value) {
    int idx = this->findIndex(value->getLibrary(), value->getIconName());

    if (idx != -1) {
      return;
    }
    
    this->data[dataLength] = value;
    dataLength += 1;
  }

  void remove(const char* libraryName, const char* iconName) {
    int idx = this->findIndex(libraryName, iconName);

    if (idx == -1) {
      return;
    }

    delete this->data[idx];
    this->data[idx] = NULL;

    for (int i = idx; i < 3; i++) {
      RasterizerData* temp = this->data[i];
      this->data[i] = this->data[i + 1];
      this->data[i + 1] = NULL;
    }

    this->dataLength -= 1;
  }

  void update() {
    displayManager.clear();
    
    int x = (128 - (this->dataLength * 32)) / 2;
    for (int i = 0; i < 4; i++) {
      if (data[i] != NULL) {
        data[i]->draw(displayManager, x + (i * 32), 0, WHITE);
      }
    }
    
    displayManager.rerender();
  }

  void loop() {
    
  }

  ~StatusDisplay() {
    for (int i = 0; i < 4; i++) {
      if (data[i] != NULL) {
        delete data[i];
      }
    }
  }
};