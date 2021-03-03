#include "Boundry.cpp"
#include "HWIO.cpp"
#include "Initializer.cpp"
#include "Logger.cpp"
#include "MessageSender.cpp"
#include "Profile.cpp"
#include "TemperatureDetection.cpp"

void setup() {
  
}

void loop() {
  // check termal range
  temperatureCompare();
  
  if (!isTemperatureInsideBoundries()) {
    // TODO message sender flag
  }

  // TODO daniel log temperature
  logData();

  // TODO daniel handle power outage
  
  // read button to change current profile
  if (profileBtnPressed()) {
    incrementProfile();
  }
}
