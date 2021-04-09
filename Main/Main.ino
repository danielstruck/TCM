#include "inc/Main.hpp"
#include "inc/Errors.h"
#include "inc/HWIO.hpp"
#include "inc/Initializer.hpp"
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/Profile.hpp"
#include "inc/TemperatureDetection.hpp"


void setup() {
  // initialize pins
  setupPins();
      
  // initialize serial stream
  setLEDs(1, 1, 1, 1, 1);
  while(!Serial);
  Serial.begin(115200);
  setLEDs(0, 0, 0, 0, 0);

  // enable and start up Fona 3G
  setupFona();

  // setup logger and SD card bus
  setupLogger();

  // ask user to input initial profile parameters
  setupInitialProfile();
  
  DEBUG_PRINTLN(F("Setup complete"));

}

void printState() {
//  DEBUG_PRINT(F("err=")); DEBUG_PRINT(errorFlag);
//  DEBUG_PRINT(F("\tprof=")); DEBUG_PRINT(currentProfile);
//  DEBUG_PRINT(F("\ttemp=")); DEBUG_PRINT(temperatureChamber);
//  DEBUG_PRINT(F("\tbound=")); DEBUG_PRINT(profile[currentProfile].lower); DEBUG_PRINT(F(","));DEBUG_PRINT(profile[currentProfile].upper);
//  DEBUG_PRINT(F("\tpwrOk=")); DEBUG_PRINT(isPowerOK());
//  DEBUG_PRINT(F("\trst=")); DEBUG_PRINT(resetBtnPressed());
//  DEBUG_PRINT(F("\tfona=")); DEBUG_PRINT(isFonaOn()? 1: 0);
//  DEBUG_PRINTLN(F(""));
}
void loop() {
  printState();
  
  // check termal range
  temp_sense();
  if (!isTemperatureInsideBoundries() || (errorFlag & bit(badTemp))) {
    DEBUG_PRINTLN(F("> Bad temperature detected"));
    setErrorFlag(badTemp);
  }
  
  logData(temperatureChamber);
      
  if (!isPowerOK() || (errorFlag & bit(badPower))) {
    bool lastPowerState = false;
    DEBUG_PRINT(F("> Power outage detected ")); DEBUG_PRINTLN(analogRead(PIN_POWER_INDICATOR));
    setErrorFlag(badPower);
    
    if (!lastPowerState && isPowerOK()) {
      sendText(powerRestored);
    }
    else if (lastPowerState && !isPowerOK()) {
      setFonaOff();
    }

    lastPowerState = isPowerOK();
  }
  
  if (resetBtnPressed() && errorFlag != 0) {
    DEBUG_PRINTLN(F("> Reset ON"));
    sendText(deviceReset);
    errorFlag = 0;
  }

  // TODO periodic report
  if (!errorFlag) {
    sendText(periodicReport);
  }
      
  // read button to change current profile
  if (isProfileBtnRising()) {
    DEBUG_PRINTLN(F("> Profile increment"));
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

  // send error SMS messages
  sendError();
}
