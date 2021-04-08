#include "inc/Main.hpp"
#include "inc/Errors.h"
#include "inc/HWIO.hpp"
#include "inc/Initializer.hpp"
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/Profile.hpp"
#include "inc/TemperatureDetection.hpp"


void setup() {
      DEBUG_PRINTLN(F("@.1"));
  // initialize pins
  setupPins();

      DEBUG_PRINTLN(F("@.2"));
      
  // initialize serial stream
  setLEDs(1, 1, 1, 1, 1);
  while(!Serial);
  Serial.begin(115200);
//  delay(5000);
  setLEDs(0, 0, 0, 0, 0);
  
      DEBUG_PRINTLN(F("@.3"));

  // enable and start up Fona 3G
  setupFona();
  
      DEBUG_PRINTLN(F("@.4"));

  // setup logger and SD card bus
  setupLogger();

      DEBUG_PRINTLN(F("@.5"));

  // ask user to input initial profile parameters
  setupInitialProfile();
  
      DEBUG_PRINTLN(F("@.6"));
  
  DEBUG_PRINTLN(F("Setup complete"));
  
      DEBUG_PRINTLN(F("@.7"));
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
      DEBUG_PRINTLN(F("@0"));
  
  printState();
  
      DEBUG_PRINTLN(F("@1"));
  
  // check termal range
  temp_sense();
      DEBUG_PRINTLN(F("@2"));
  if (!isTemperatureInsideBoundries() || (errorFlag & bit(badTemp))) {
    DEBUG_PRINTLN(F("> Bad temperature detected"));
    setErrorFlag(badTemp);
  }
      DEBUG_PRINTLN(F("@3"));
  
  logData(temperatureChamber);

      DEBUG_PRINTLN(F("@4"));
      
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

      DEBUG_PRINTLN(F("@5"));
  
  if (resetBtnPressed() && errorFlag != 0) {
    DEBUG_PRINTLN(F("> Reset ON"));
    sendText(deviceReset);
    errorFlag = 0;
  }
  
      DEBUG_PRINTLN(F("@6"));

  // TODO periodic report
  if (!errorFlag) {
    sendText(periodicReport);
  }
  
      DEBUG_PRINTLN(F("@7"));
      
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
