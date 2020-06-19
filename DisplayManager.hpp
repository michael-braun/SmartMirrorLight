#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

class DisplayManager {
private:
  Adafruit_SSD1306* display;

public:
  DisplayManager() {
    this->display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  }

  void setup() {
    this->display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    this->display->display();
  }

  void clear() {
    this->display->clearDisplay();
    this->rerender();
  }

  void rerender() {
    this->display->display();
  }

  void drawBitmap(int16_t x, int16_t y, const char bitmap[], int16_t width, int16_t height, uint16_t color) {
    this->drawBitmap(x, y, (const uint8_t*) bitmap, width, height, color);
  }

  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t width, int16_t height, uint16_t color) {
    this->display->drawBitmap(x, y, bitmap, width, height, color);
  }

  void setText(int16_t x, int16_t y, const char* data) {
    this->display->setCursor(x, y);
    this->display->setTextColor(WHITE);
    this->display->setTextSize(2);
    this->display->print(data);
  }

  ~DisplayManager() {
    delete this->display;
  }
};
