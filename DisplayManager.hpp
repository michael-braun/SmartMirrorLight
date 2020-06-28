#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "./TCASelectHelper.hpp"
#include "./FontOptions.hpp"

class DisplayManager : public TCASelectHelper {
public:
  DisplayManager() {
  }

  virtual void setup() {
  }

  virtual void clear() {
  }

  virtual void rerender() {
  }

  virtual void command(uint8_t c) {
  }

  virtual void drawBitmap(int16_t x, int16_t y, const char bitmap[], int16_t width, int16_t height, uint16_t color) {
  }

  virtual void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t width, int16_t height, uint16_t color) {
  }

  virtual void drawCircle(int16_t x, int16_t y, uint16_t radius, uint16_t color) {
  }

  virtual void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
  }

  virtual void setText(int16_t x, int16_t y, const char* data, const FontOptions* fontOptions) {
  }

  virtual void enable() {
    
  }

  virtual void disable() {
    
  }

  ~DisplayManager() {
  }
};

#endif
