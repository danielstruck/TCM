#include "inc/Main.hpp"
#include "inc/Errors.h"
#include "inc/HWIO.hpp"
#include "inc/Initializer.hpp"
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/Profile.hpp"
#include "inc/TemperatureDetection.hpp"


void sendSMSWithError(int errorCode); // TODO move this to file MessageSender?


void setup() {
  // initialize pins
  setupPins();

  // initialize serial stream
  setLEDs(1, 1, 1, 1, 1);
  while(!Serial);
  Serial.begin(115200);
  delay(5000);
  setLEDs(0, 0, 0, 0, 0);

  // enable and start up Fona 3G
  setupFona();

  // setup logger and SD card bus
  setupLogger();

  // ask user to input initial profile parameters
  setupInitialProfile();
  
  
  Serial.println("Setup complete");
}

void printState() {
  Serial.print("err="); Serial.print(errorFlag);
  Serial.print("prof="); Serial.print(currentProfile);
  Serial.print("temp="); Serial.print(temperatureChamber);
  Serial.print("bound="); Serial.print(profile[currentProfile].lower);Serial.print(",");Serial.print(profile[currentProfile].upper);
  Serial.print("pwrOk="); Serial.print(isPowerOK());
  Serial.print("rst="); Serial.print(resetBtnPressed());
  Serial.print("fonaOn="); Serial.print(isFonaOn());
  Serial.println("");
}
void loop() {
//  char stateStr[128];
//  sprintf(stateStr, "err=%-3d  prof=%d  tmp=%-6d  bounds=%-3d,%-3d  pwrOK=%-2d  rst=%-2d  fonaOn=%-2d",
//          errorFlag, currentProfile, temperatureChamber, profile[currentProfile].lower, profile[currentProfile].upper, isPowerOK(), resetBtnPressed(), isFonaOn());
//  Serial.println(stateStr);
  printState();
  
  
  // check termal range
  temp_sense();
  if (!isTemperatureInsideBoundries() || (errorFlag & bit(badTemp))) {
    Serial.println("> Bad temperature detected");
    sendSMSWithError(badTemp);
  }
  
  logData(temperatureChamber);

  if (!isPowerOK() || (errorFlag & bit(badPower))) {
    Serial.print("> Power outage detected "); Serial.println(analogRead(PIN_POWER_INDICATOR));
    sendSMSWithError(badPower);
    
    if (isPowerOK()) {
      sendText(powerRestored);
    }
  }

  
  if (resetBtnPressed()) {
    Serial.println("> Reset ON");
    sendText(deviceReset);
    errorFlag = 0;
  }

  // TODO periodic report
  sendText(periodicReport);
  
  // read button to change current profile
  if (isProfileBtnRising()) {
    Serial.println("> Profile increment");
    incrementProfile();
    sendText(profileSwitched);
  
    switch (currentProfile) {
      case 0: 
        setLEDs(0, 1, 0, 0, 0);
      break;
      case 1: 
        setLEDs(0, 0, 1, 0, 0);
      break;
      case 2: 
        setLEDs(0, 0, 0, 1, 0);
      break;
      case 3: 
        setLEDs(0, 0, 0, 0, 1);
      break;
      default:
        setLEDs(0, 1, 1, 1, 1);
    }
  }

  // TODO (stretch goal) receive SMS


  // display any error codes on the error LED
  blinkLED();
}


void sendSMSWithError(int eventCode) {
    setErrorFlag(eventCode);
    sendText(eventCode);
}
