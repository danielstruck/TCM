#include "inc/Main.hpp"
#include "inc/Errors.h"
#include "inc/HWIO.hpp"
#include "inc/Initializer.hpp"
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/Profile.hpp"
#include "inc/TemperatureDetection.hpp"


void sendSMSWithError(int errorCode); // TODO move this to file MessageSender?
void sendSMS(int errorCode); // TODO move this to file MessageSender?


void setup() {
  // initialize pins
  setupPins();

  // initialize serial stream
  setLEDs(1, 1, 1, 1, 1);
  while(!Serial);
  Serial.begin(115200);
  delay(5000);

  // enable and start up Fona 3G
  setLEDs(0, 1, 0, 0, 0);
  setupFona();

  // setup logger and SD card bus
  setLEDs(0, 1, 1, 0, 0);
  setupLogger();

  // ask user to input initial profile parameters
//  setupInitialProfile();

  // turn off error LED
  setLEDs(0, -1, -1, -1, -1);
  
  
  Serial.println("Setup complete");
}

void loop() {
  char stateStr[128];
  sprintf(stateStr, "err=%-3d  tmp=%-6d  pwrOK=%-2d  rst=%-2d  rtc=%",
          errorFlag, isTemperatureInsideBoundries(), isPowerOK(), resetBtnPressed());
  Serial.println(stateStr);
  
  logData(temperatureChamber);
  
  // check termal range
//  if (!isTemperatureInsideBoundries() || (errorFlag & bit(badTemp))) {
//    sendSMSWithError(badTemp);
//  }

  if (!isPowerOK() || (errorFlag & bit(badPower))) {
    Serial.println("Power outage detected");
    sendSMSWithError(badPower);
  }

  
  if (resetBtnPressed()) {
    errorFlag = 0;
  }

  // TODO periodic report
  
  // read button to change current profile
  if (isProfileBtnRising()) {
    incrementProfile();
  }

  // TODO (stretch goal) receive SMS
}


void sendSMSWithError(int eventCode) {
    setErrorFlag(eventCode);
    sendSMS(eventCode);
    blinkLED(eventCode);
}

void sendSMS(int eventCode) {
    chooseMessage(eventCode);
    sendText();
}
