#include <SparkFun_APDS9960.h>

using f_void_t = void(*)();

class GestureManager {
private:
  SparkFun_APDS9960 apds = SparkFun_APDS9960();

  f_void_t onLeft = NULL;
  f_void_t onRight = NULL;
  f_void_t onUp = NULL;
  f_void_t onDown = NULL;
  f_void_t onNear = NULL;
  f_void_t onFar = NULL;
  
public:
  void setOnLeft(f_void_t cb) {
    this->onLeft = cb;
  }

  void setOnRight(f_void_t cb) {
    this->onRight = cb;
  }

  void setOnUp(f_void_t cb) {
    this->onUp = cb;
  }

  void setOnDown(f_void_t cb) {
    this->onDown = cb;
  }

  void setOnNear(f_void_t cb) {
    this->onNear = cb;
  }

  void setOnFar(f_void_t cb) {
    this->onFar = cb;
  }

  void setup() {
    if ( apds.init() ) {
      Serial.println(F("APDS-9960 initialization complete"));
    } else {
      Serial.println(F("Something went wrong during APDS-9960 init!"));
    }
  
    if ( apds.enableGestureSensor(true) ) {
      Serial.println(F("Gesture sensor is now running"));
    } else {
      Serial.println(F("Something went wrong during gesture sensor init!"));
    }
  }

  void loop() {
    Serial.println("loop");
    if ( apds.isGestureAvailable() ) {
      Serial.println("gesture available");
      Serial.println(apds.readGesture());
//      switch ( apds.readGesture() ) {
//        case DIR_UP:
//          Serial.println("up");
//          if (this->onUp != NULL) {
//            this->onUp();
//          }
//          break;
//        case DIR_DOWN:
//          Serial.println("down");
//          if (this->onDown != NULL) {
//            this->onDown();
//          }
//          break;
//        case DIR_LEFT:
//          Serial.println("left");
//          if (this->onLeft != NULL) {
//            this->onLeft();
//          }
//          break;
//        case DIR_RIGHT:
//          Serial.println("right");
//          if (this->onRight != NULL) {
//            this->onRight();
//          }
//          break;
//        case DIR_NEAR:
//          Serial.println("near");
//          if (this->onNear != NULL) {
//            this->onNear();
//          }
//          break;
//        case DIR_FAR:
//          Serial.println("far");
//          if (this->onFar != NULL) {
//            this->onFar();
//          }
//          break;
//        default:
//          Serial.println("none");
//          break;
//      }
//      Serial.println("end available");
    }
  }
};
