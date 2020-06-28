#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "./TCASelectHelper.hpp"
#include "./FontOptions.hpp"

#define U8G2_TYPE_SSD1306 1
#define U8G2_TYPE_SH1106 2

class U8G2DisplayManager : public DisplayManager {
private:
  U8G2* display;

public:
  U8G2DisplayManager(unsigned char type, int screenWidth, int screenHeight) : DisplayManager() {
    Serial.begin(9600);
    Serial.println("use U8G2DisplayManager");
    if (type == U8G2_TYPE_SH1106) {
      if (screenWidth == 128 && screenHeight == 64) {
        this->display = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
        return;
      }
    }
    
    if (type == U8G2_TYPE_SSD1306) {
      if (screenWidth == 128 && screenHeight == 64) {
        Serial.println("use display");
        this->display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
        return;
      }
    }

    this->display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
  }

  void setup() {
    this->updateTCAPort();
    this->display->begin();
  }

  void clear() {
    this->updateTCAPort();
    Serial.println("clear");
    this->display->clearBuffer();
  }

  void setContrast(uint8_t contrast) {
    this->display->setContrast(contrast);
  }

  void rerender() {
    this->updateTCAPort();
    Serial.println("rerender");
    Serial.println((int) this->display);
    this->display->sendBuffer();
  }

  void command(uint8_t c) {

  }

  void drawBitmap(int16_t x, int16_t y, const char bitmap[], int16_t width, int16_t height, uint16_t color) {
    this->drawBitmap(x, y, (const uint8_t*) bitmap, width, height, color);
  }

  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t width, int16_t height, uint16_t color) {
    this->display->drawBitmap(x, y, (width + 7) / 8, height, bitmap);
  }

  void drawCircle(int16_t x, int16_t y, uint16_t radius, uint16_t color) {
    this->display->drawCircle(x, y, radius, U8G2_DRAW_ALL);
  }

  void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {

  }

  void setText(int16_t x, int16_t y, const char* data, const FontOptions* fontOptions) {
    this->display->setFont(u8g2_font_logisoso20_tr);

    int xOffset = 0;

    if (fontOptions->textAlign == TEXT_ALIGN_RIGHT) {
      unsigned int width = this->display->getStrWidth(data);
      xOffset = -width;
    } else if(fontOptions->textAlign == TEXT_ALIGN_CENTER) {
      unsigned int width = this->display->getStrWidth(data);
      xOffset = -(width / 2);
    } else {
      xOffset = 0;
    }
    
    this->display->drawStr(x + xOffset, y, data);
  }

  void enable() {
    this->display->setPowerSave(0);
  }

  void disable() {
    this->display->setPowerSave(1);
  }

  ~U8G2DisplayManager() {
    delete this->display;
  }
};
