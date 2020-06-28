#define ORIENTATION_NORMAL 1
#define ORIENTATION_INVERSE -1

#define ANIMATION_HIDE 1
#define ANIMATION_SHOW 2
#define ANIMATION_DISABLE 4
#define ANIMATION_ENABLE 8

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

  uint8_t nextValue[4] = { 0x0, 0x0, 0x0, 0x0 };

  const uint8_t emptyValue[4] = { 0x0, 0x0, 0x0, 0x0 };

  uint8_t brightness = 0;

  bool orientation = true;

  bool dots = false;

  unsigned int transition = 0;

  bool bIsAnimating = false;

  unsigned int animationStart = 0;

  unsigned int animationStep = 0;

  unsigned int animationLastStepTime = 0;

  char animationType = 0;

  boolean bIsEnabled = true;

  void applyNextValue() {
    for(int i = 0; i < 4; i++) {
      if (this->dots) {
        this->nextValue[i] = (0b10000000 | this->nextValue[i]);
      } else {
        this->nextValue[i] = (0b01111111 & this->nextValue[i]);
      }
    }

    this->currentValue[0] = this->nextValue[0];
    this->currentValue[1] = this->nextValue[1];
    this->currentValue[2] = this->nextValue[2];
    this->currentValue[3] = this->nextValue[3];

    this->update();
  }

public:
  DigitDisplayManager(const int clkPin, const int dioPin) : display(clkPin, dioPin) {
  }

  void disable() {
    this->bIsEnabled = false;
    
    if (this->transition) {
      this->bIsAnimating = true;
      this->animationStep = 7;
      this->animationStart = millis();
      this->animationType = ANIMATION_DISABLE;
      this->animationLastStepTime = this->animationStart;
    }
  }

  void enable() {
    this->bIsEnabled = true;
    
    if (this->transition) {
      this->bIsAnimating = true;
      this->animationStep = 1;
      this->animationStart = millis();
      this->animationType = ANIMATION_SHOW;
      this->animationLastStepTime = this->animationStart;
    }
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

  bool isAnimating() {
    return this->bIsAnimating;
  }

  void loop() {
    if (!this->isAnimating()) {
      return;
    }

    if (millis() - this->animationLastStepTime > (this->transition / 7)) {
      if (this->animationType == ANIMATION_HIDE || this->animationType == ANIMATION_DISABLE) {
        this->setBrightness(this->animationStep);

        if (this->animationStep <= 0) {
          display.setSegments(this->emptyValue);

          if (this->animationType == ANIMATION_HIDE) {
            this->applyNextValue();
  
            this->animationType = ANIMATION_SHOW;
            this->animationLastStepTime = millis();
            this->animationStep = 1;
          }

          if (this->animationType == ANIMATION_DISABLE) {
            this->bIsAnimating = false;
          }
          return;
        }

        this->animationLastStepTime = millis();
        this->animationStep -= 1;
        return;
      }
      
      if (this->animationType == ANIMATION_SHOW || this->animationType == ANIMATION_ENABLE) {
        this->setBrightness(this->animationStep);

        if (this->animationStep >= 7) {
          this->bIsAnimating = false;
          this->animationType = 0;
          this->animationLastStepTime = millis();
          this->animationStep = 0;
          return;
        }

        this->animationLastStepTime = millis();
        this->animationStep += 1;
        return;
      }
    }
  }

  void animate() {
    while(this->isAnimating()) {
      this->loop();
      delay((this->transition / 7) + 1);
    }
  }

  void setSegments(const uint8_t data[4]) {
    if (this->transition && this->bIsEnabled) {
      this->bIsAnimating = true;
      this->animationStep = 7;
      this->animationStart = millis();
      this->animationType = ANIMATION_HIDE;
      this->animationLastStepTime = this->animationStart;
    }

    if (this->orientation) {
      this->nextValue[0] = data[0];
      this->nextValue[1] = data[1];
      this->nextValue[2] = data[2];
      this->nextValue[3] = data[3];
    } else {
      uint8_t d1 = this->invertDigit(data[3]);
      uint8_t d2 = this->invertDigit(data[2]);
      uint8_t d3 = this->invertDigit(data[1]);
      uint8_t d4 = this->invertDigit(data[0]);
      
      this->nextValue[0] = d1;
      this->nextValue[1] = d2;
      this->nextValue[2] = d3;
      this->nextValue[3] = d4;
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
