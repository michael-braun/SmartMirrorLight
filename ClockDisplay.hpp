#include "./ClockHelper.hpp"

const int CLK = D6; //Set the CLK pin connection to the display
const int DIO = D5; //Set the DIO pin connection to the display


class ClockDisplay {
private:
  DigitDisplayManager digitDisplayManager = { CLK, DIO }; //set up the 4-Digit Display.
  ClockHelper clockHelper;
  int lastClock = 0;
  unsigned int lastUpdate = 0;

public:
  void setup() {
    clockHelper.setup();

    digitDisplayManager.setOrientation(ORIENTATION_INVERSE);
    digitDisplayManager.setDots(true);
    
    digitDisplayManager.setTransition(500);
    digitDisplayManager.setBrightness(0);
  }

  void loop() {
    digitDisplayManager.loop();
    
    if (!digitDisplayManager.isAnimating() && (millis() - lastUpdate) > 10000) {
      clockHelper.loop();
      
      int epoch = clockHelper.getEpochTime();
      int hours = ((epoch % 86400L) / 3600);
      int minutes = ((epoch % 3600) / 60);
  
      int newClock = hours * 100 + minutes;
  
      if (newClock != lastClock) {
        digitDisplayManager.showNumberDec(newClock, true); //Display the numCounter value;
        digitDisplayManager.animate();
  
        lastClock = newClock;
      }
  
      lastUpdate = millis();
    }
  }
};
