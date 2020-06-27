#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "./TCASelectHelper.hpp"
#include "./FontOptions.hpp"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

class DisplayManager : public TCASelectHelper {
private:
  Adafruit_SSD1306* display;

public:
  DisplayManager(int screenWidth, int screenHeight) {
    this->display = new Adafruit_SSD1306(screenWidth, screenHeight, &Wire, OLED_RESET);
  }

  void setup() {
    this->updateTCAPort();
    this->display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    this->display->display();
  }

  void clear() {
    this->display->clearDisplay();
    this->rerender();
  }

  void rerender() {
    this->updateTCAPort();
    this->display->display();
  }

  void command(uint8_t c) {
    this->updateTCAPort();
    this->display->ssd1306_command(c);
  }

  void drawBitmap(int16_t x, int16_t y, const char bitmap[], int16_t width, int16_t height, uint16_t color) {
    this->updateTCAPort();
    this->drawBitmap(x, y, (const uint8_t*) bitmap, width, height, color);
  }

  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t width, int16_t height, uint16_t color) {
    this->updateTCAPort();
    this->display->drawBitmap(x, y, bitmap, width, height, color);
  }

  void drawCircle(int16_t x, int16_t y, uint16_t radius, uint16_t color) {
    this->updateTCAPort();
    this->display->drawCircle(x, y, radius, color);
  }

  void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    this->updateTCAPort();
    this->display->drawRect(x, y, w, h, color);
  }

  void setText(int16_t x, int16_t y, const char* data, const FontOptions* fontOptions) {
    this->updateTCAPort();
    this->display->setTextColor(fontOptions->fontColor);
    this->display->setTextSize(fontOptions->fontSize);

    if (fontOptions->textAlign == TEXT_ALIGN_RIGHT) {
      int16_t xOut, yOut;
      uint16_t wOut, hOut;
      
      this->display->getTextBounds(data, 0, 0, &xOut, &yOut, &wOut, &hOut);
      this->display->setCursor(x - wOut, y);
    } else if(fontOptions->textAlign == TEXT_ALIGN_CENTER) {
      int16_t xOut, yOut;
      uint16_t wOut, hOut;
      
      this->display->getTextBounds(data, 0, 0, &xOut, &yOut, &wOut, &hOut);
      this->display->setCursor(x - (wOut / 2), y);
    } else {
      this->display->setCursor(x, y);
    }
    
    this->display->print(data);
  }

  ~DisplayManager() {
    delete this->display;
  }
};
