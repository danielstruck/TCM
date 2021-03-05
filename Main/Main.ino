#include "inc/Main.hpp"
#include "inc/Boundry.hpp"
#include "inc/HWIO.hpp"
#include "inc/Initializer.hpp"
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/Profile.hpp"
#include "inc/TemperatureDetection.hpp"

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
