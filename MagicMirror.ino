#include <TM1637Display.h>
#include "./DigitDisplayManager.hpp"
 
const int CLK = D6; //Set the CLK pin connection to the display
const int DIO = D5; //Set the DIO pin connection to the display
 
int numCounter = 0;
 
DigitDisplayManager displayManager(CLK, DIO); //set up the 4-Digit Display.
 
void setup()
{
  Serial.begin(9600);
  Serial.println("Test");

  displayManager.setOrientation(ORIENTATION_INVERSE);
  displayManager.setDots(true);

  displayManager.setTransition(500);
  displayManager.setBrightness(0);
  delay(1000);
}

void loop()
{
  displayManager.loop();

  //displayManager.animate();
  if (!displayManager.isAnimating()) {
    displayManager.showNumberDec(numCounter, true); //Display the numCounter value;
    
    delay(1000);

    numCounter++;
  }
}
