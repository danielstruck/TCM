#include "inc/Main.hpp"
#include "inc/Boundry.hpp"
#include "inc/HWIO.hpp"
#include "inc/Initializer.hpp"
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/Profile.hpp"
#include "inc/TemperatureDetection.hpp"


void setup() {
  setupPins();
  setupLogger();
}

void loop() {
  // check termal range
  if (!isTemperatureInsideBoundries()) {
    // TODO message sender flag
  }

  // TODO daniel log temperature
  logData(temperatureChamber);

  // TODO daniel handle power outage
  
  // read button to change current profile
  if (profileBtnPressed()) {
    incrementProfile();
  }

  if (!isPowerOK()) {
//    chooseMessage(events.badPower);
    sendText();
  }
}
