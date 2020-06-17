#define ORIENTATION_NORMAL 1
#define ORIENTATION_INVERSE -1

const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
};

class DigitDisplayManager {
private:
  TM1637Display display;

  uint8_t currentValue[4] = { 0x0, 0x0, 0x0, 0x0 };

  const uint8_t emptyValue[4] = { 0x0, 0x0, 0x0, 0x0 };

  uint8_t brightness = 0;

  bool orientation = true;

  bool dots = false;

  unsigned int transition = 0;

public:
  DigitDisplayManager(const int clkPin, const int dioPin) : display(clkPin, dioPin) {
  }

  void update() {
    display.setBrightness(this->brightness);
    display.setSegments(this->currentValue);
  }

  void setTransition(unsigned int transition) {
    this->transition = transition;
  }

  void setOrientation(int orientation) {
    Serial.print("set orientation:");
    Serial.println((int) orientation);
    this->orientation = (orientation != -1 ? true : false);
  }

  void setBrightness(uint8_t brightness) {
    this->brightness = brightness;
    
    this->update();
  }

  void setDots(bool enabled) {
    this->dots = enabled;

    this->update();
  }

  void setSegments(const uint8_t data[4]) {
    if (this->transition) {
      unsigned int stepTime = (this->transition / 7);

      for(int i = 7; i >= 0; i--) {
        this->setBrightness(i);
        delay(stepTime);
      }

      display.setSegments(this->emptyValue);
    }
    
    if (this->orientation) {
      this->currentValue[0] = data[0];
      this->currentValue[1] = data[1];
      this->currentValue[2] = data[2];
      this->currentValue[3] = data[3];
    } else {
      uint8_t d1 = this->invertDigit(data[3]);
      uint8_t d2 = this->invertDigit(data[2]);
      uint8_t d3 = this->invertDigit(data[1]);
      uint8_t d4 = this->invertDigit(data[0]);
      
      this->currentValue[0] = d1;
      this->currentValue[1] = d2;
      this->currentValue[2] = d3;
      this->currentValue[3] = d4;
    }

    for(int i = 0; i < 4; i++) {
      if (this->dots) {
        this->currentValue[i] = (0b10000000 | this->currentValue[i]);
      } else {
        this->currentValue[i] = (0b01111111 & this->currentValue[i]);
      }
    }
    
    this->update();

    if (this->transition) {
      unsigned int stepTime = (this->transition / 7);
      
      for(int i = 0; i <= 7; i++) {
        this->setBrightness(i);
        delay(stepTime);
      }
    }
  }

  void showNumberDec(unsigned int num, bool leadingZero = false) {
    uint8_t newValue[4] = { this->currentValue[0], this->currentValue[1], this->currentValue[2], this->currentValue[3] };
    
    this->encodeDigits(num, newValue, leadingZero);
    this->setSegments(newValue);
  }

  void encodeDigits(unsigned int value, uint8_t data[4], bool leadingZero) {
    int vData = value;
    int pot = 1;
    
    for (int i = 1; i <= 4; i++) {
      vData = (value / pot) - ((value / (pot * 10)) * 10);

      if (!leadingZero && !(value / pot)) {
        data[4 - i] = 0x0;
      } else {
        data[4 - i] = encodeDigit(vData); 
      }

      pot *= 10;
    }
  }

  uint8_t encodeDigit(int value) {
    return digitToSegment[value];
  }

  uint8_t invertDigit(uint8_t value) {
    uint8_t invertedValue = 0;

    invertedValue |= (value & 0b01000000);

    invertedValue |= ((value & 0b00000001) << 3);
    invertedValue |= ((value & 0b00000010) << 3);
    invertedValue |= ((value & 0b00000100) << 3);

    invertedValue |= ((value & 0b00001000) >> 3);
    invertedValue |= ((value & 0b00010000) >> 3);
    invertedValue |= ((value & 0b00100000) >> 3);

    return invertedValue;
  }

  void clear() {
    this->currentValue[0] = 0;
    this->currentValue[1] = 0;
    this->currentValue[2] = 0;
    this->currentValue[3] = 0;
    
    this->update();
  }
};
