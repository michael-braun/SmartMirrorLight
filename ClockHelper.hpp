#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class ClockHelper {
private:
  WiFiUDP ntpUDP;
  NTPClient* timeClient;

public:
  ClockHelper() {
    this->timeClient = new NTPClient(this->ntpUDP, "pool.ntp.org", TIME_ZONE_OFFSET);
  }

  void setup() {
    timeClient->begin();
  }

  void loop() {
    timeClient->update();
  }

  int getEpochTime() const {
    return this->timeClient->getEpochTime() + (TIME_ZONE_OFFSET * 3600);
  }

  ~ClockHelper() {
    delete this->timeClient;
  }
};
