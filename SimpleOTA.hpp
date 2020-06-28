#include <ArduinoOTA.h>

class SimpleOTA {
private:
  String version; 
  
public:
  SimpleOTA(const char* version) {
    this->version = version;
  }

  void setup() {
    ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { // U_FS
        type = "filesystem";
      }
  
      // NOTE: if updating FS this would be the place to unmount FS using FS.end()
      if (Serial) {
        Serial.println("Start updating " + type);
      }
    });
  
    ArduinoOTA.onEnd([]() {
      if (Serial) {
        Serial.println("\nEnd");
      }
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      if (Serial) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      }
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
      if (!Serial) {
        return;
      }
      
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
    
    ArduinoOTA.begin();

    if(Serial) {
      Serial.print("Current version: ");
      Serial.print(version);
      Serial.print(" (");
      Serial.print(__DATE__);
      Serial.print(" ");
      Serial.print(__TIME__);
      Serial.println(")");
    }
  }

  void loop() {
    ArduinoOTA.handle();
  }
};
