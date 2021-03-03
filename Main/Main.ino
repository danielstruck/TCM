//#include "../inc/Main.hpp"
//#include "Bounries.ino"
//#include "HWIO.ino"
//#include "Initializer.ino"
//#include "Logger.ino"
//#include "MessageSender.ino"
//#include "Profile.ino"
//#include "Temperature.ino"



void setup() {
  
}

void loop() {
  // check termal range
  temperatureCompare();
  
  if (!temperatureInsideBoundries) {
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
