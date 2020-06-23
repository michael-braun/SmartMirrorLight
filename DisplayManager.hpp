#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define TEXT_ALIGN_LEFT 1
#define TEXT_ALIGN_CENTER 2
#define TEXT_ALIGN_RIGHT 3

class FontOptions {
public:
  const GFXfont* font;
  uint8_t fontSize;
  uint8_t fontColor;
  char textAlign;

public:
  FontOptions(const GFXfont *font = NULL, uint8_t size = 1, uint16_t color = WHITE, char textAlign = TEXT_ALIGN_LEFT) {
    this->font = font;
    this->fontSize = size;
    this->fontColor = color;
    this->textAlign = textAlign;
  }
};

class DisplayManager {
private:
  Adafruit_SSD1306* display;

public:
  DisplayManager(int screenWidth, int screenHeight) {
    this->display = new Adafruit_SSD1306(screenWidth, screenHeight, &Wire, OLED_RESET);
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

  void command(uint8_t c) {
    this->display->ssd1306_command(c);
  }

  void drawBitmap(int16_t x, int16_t y, const char bitmap[], int16_t width, int16_t height, uint16_t color) {
    this->drawBitmap(x, y, (const uint8_t*) bitmap, width, height, color);
  }

  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t width, int16_t height, uint16_t color) {
    this->display->drawBitmap(x, y, bitmap, width, height, color);
  }

  void drawCircle(int16_t x, int16_t y, uint16_t radius, uint16_t color) {
    this->display->drawCircle(x, y, radius, color);
  }

  void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    this->display->drawRect(x, y, w, h, color);
  }

  void setText(int16_t x, int16_t y, const char* data, const FontOptions* fontOptions) {
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
