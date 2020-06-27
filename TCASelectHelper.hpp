
#ifndef TCA_SELECT_HELPER_H
#define TCA_SELECT_HELPER_H

class TCASelectHelper {
private:
  int16_t tcaPort = -1;

public:
  void setTCAPort(uint8_t port) {
    this->tcaPort = port;
  }

  void updateTCAPort() {
    if (this->tcaPort >= 0 && this->tcaPort <= 7) {
      Wire.begin(D3, D4);
      
      Wire.beginTransmission(0x70);
      Wire.write(1 << this->tcaPort);
      Wire.endTransmission();
    }
  }
};

#endif
