#include "inc/Main.hpp"
#include "inc/Errors.h"
#include "inc/HWIO.hpp"
#include "inc/Initializer.hpp"
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/Profile.hpp"
#include "inc/TemperatureDetection.hpp"
#include <SD.h>

void setup() {
  // initialize pins
  setupPins();
  setLEDs(1, 1, 1, 1, 1); // show user that the program is in setup
  
  // initialize serial stream
  while(!Serial);
  Serial.begin(115200);

  // enable and start up Fona 3G
  setupFona();

  // setup logger and SD card bus
  setupLogger();
  setLEDs(0, 0, 0, 0, 0); // show user that the program is no longer in setup

  // ask user to input initial profile parameters
  setupInitialProfile();
  
  DEBUG_PRINTLN(F("Setup complete"));
}

void printState() {
  DEBUG_PRINT(F("err=")); DEBUG_PRINT(errorFlag);
  DEBUG_PRINT(F("\tprof=")); DEBUG_PRINT(currentProfile);
  DEBUG_PRINT(F("\ttemp=")); DEBUG_PRINT((int) temperatureChamber);
  DEBUG_PRINT(F("\tbound=")); DEBUG_PRINT((int) profile[currentProfile].lower); DEBUG_PRINT(F(","));DEBUG_PRINT((int) profile[currentProfile].upper);
  DEBUG_PRINT(F("\tpwrOk=")); DEBUG_PRINT(isPowerOK());
  DEBUG_PRINT(F("\trst=")); DEBUG_PRINT(resetBtnPressed());
//  DEBUG_PRINT(F("\tfona=")); DEBUG_PRINT(isFonaOn()? 1: 0);
  DEBUG_PRINTLN(F(""));
}
void loop() {
  static bool lastPowerState = true;
  
  printState();
  
  // check termal range
  temp_sense();
  if (!isTemperatureInsideBoundries() || (errorFlag & bit(badTemp))) {
    DEBUG_PRINTLN(F("> Bad temperature detected"));
    setErrorFlag(badTemp);
    sendError();
  }
  
  logData(temperatureChamber);
      
  if (!isPowerOK() || (errorFlag & bit(badPower))) {
    setErrorFlag(badPower);
    sendError();
    
    if (!lastPowerState && isPowerOK()) {
      sendText(powerRestored);
      setFonaOn();
      DEBUG_PRINTLN(F("Power Restored"));
    }
    if (lastPowerState && !isPowerOK()) {
      setFonaOff();
      DEBUG_PRINTLN(F("POWER FALL"));
    }
  }
  lastPowerState = isPowerOK();
  
  if (resetBtnPressed() && errorFlag != 0) {
    DEBUG_PRINTLN(F("> Reset ON"));
    sendText(deviceReset);
    sms_resetError();
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
  
    int prf = currentProfile;
    setLEDs(0, prf == 0,
               prf == 1,
               prf == 2,
               prf == 3);
  }

  // TODO (stretch goal) receive SMS
//  while (fona.getNumSMS() > 0) {
//    char *str;
//    uint16_t strLen;
//    fona.readSMS(0, str, 128, &strLen);
//    Serial.println(str);
//    fona.deleteSMS(0);
//  }

  // display any error codes on the error LED
  blinkLED();

  sendMessageToNextRecipient();
}
