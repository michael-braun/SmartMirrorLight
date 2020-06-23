#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifndef RASTERIZER_DATA_H
#define RASTERIZER_DATA_H

class RasterizerData {
private:
  char* data;
  int width;
  int height;
  const char* library;
  const char* iconName;

public:
  RasterizerData(int length, WiFiClient& client, int width, int height, const char* library, const char* iconName) {
    this->data = (char*) malloc(length * sizeof(char));

    this->width = width;
    this->height = height;

    this->library = library;
    this->iconName = iconName;

    Serial.println(this->width);
    Serial.println(this->height);

    int i = 0;
    while (client.connected() || client.available()) {
      if (client.available()) {
        this->data[i++] = client.read();   
      }
    }
  }

  const uint8_t* getBitmap() const {
    return (const uint8_t*) this->data;
  }

  const char* getLibrary() const {
    return this->library;
  }

  const char* getIconName() const {
    return this->iconName;
  }

  void draw(DisplayManager& displayManager, int16_t x, int16_t y, uint16_t color) const {
    displayManager.drawBitmap(x, y, this->getBitmap(), this->width, this->height, color);
  }

  ~RasterizerData() {
    free(this->data);
  }
};

const char* HEADER_KEYS[] = {"X-Image-Width", "X-Image-Height"};
const size_t HEADER_COUNT = 2;

class RasterizerClient {
private:
  String url;

  RasterizerData* getIcon(const char* library, const char* iconName, const String& url) const {
    HTTPClient http;

    http.collectHeaders(HEADER_KEYS, HEADER_COUNT);

    http.begin(url);
    int httpCode = http.GET();
    int bodySize = http.getSize();

    WiFiClient& client = http.getStream();

    int width = http.header("X-Image-Width").toInt();
    int height = http.header("X-Image-Height").toInt();

    auto* rasterizerData = new RasterizerData(bodySize, client, width, height, library, iconName);

    http.end();

    return rasterizerData;
  }

public: 
    RasterizerClient(const char* url) {
      this->url = url;
    }

    RasterizerData* getIcon(const char* library, const char* iconName, int width, int height) const {
      return this->getIcon(library, iconName, this->url + "/rasterizations/monochrome-bitmaps/" + library + "/" + iconName + "_w-" + String(width) + "_h-" + String(height) + ".raw");
    }

    RasterizerData* getIcon(const char* library, const char* iconName, int size) const {
      return this->getIcon(library, iconName, this->url + "/rasterizations/monochrome-bitmaps/" + library + "/" + iconName + "_s-" + String(size) + ".raw");
    }
};

#endif
