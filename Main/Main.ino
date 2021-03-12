#include "inc/Main.hpp"
#include "inc/Boundry.hpp"
#include "inc/HWIO.hpp"
#include "inc/Initializer.hpp"
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/Profile.hpp"
#include "inc/TemperatureDetection.hpp"


void setup() {
  setLEDs(0, 1, 1, 1, 1);
  Serial.begin(115200);
  setLEDs(0, 0, 1, 1, 1);
  delay(5000);
  setLEDs(0, 1, 0, 1, 1);
  setupFona();
  setLEDs(0, 0, 0, 1, 1);
  setupPins();
  setLEDs(0, 1, 1, 0, 1);
  setupLogger();
  setLEDs(1, 0, 0, 0, 0);

  setupInitialProfile();
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
