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

//  int i = 0;
//  long start = millis();
//  File f1;
//  SD.open("tester.txt", FILE_WRITE).close();
//  while (1) {
//    f1 = SD.open("tester.txt", FILE_WRITE);
//    f1.print("12345678901234567890"); f1.print(" ");
//    f1.print("12345678901234567890"); f1.print(" ");
//    f1.println("12345678901234567890");
//    f1.close();
//    DEBUG_PRINT(++i); DEBUG_PRINT(" "); DEBUG_PRINTLN(millis() - start); 
//  }
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
  static bool lastPowerState = false;
  
  printState();
  
  // check termal range
  temp_sense();
  if (!isTemperatureInsideBoundries() || (errorFlag & bit(badTemp))) {
    DEBUG_PRINTLN(F("> Bad temperature detected"));
    setErrorFlag(badTemp);
  }
  
  logData(temperatureChamber);
      
  if (!isPowerOK() || (errorFlag & bit(badPower))) {
//    DEBUG_PRINT(F("> Power outage detected ")); /*DEBUG_PRINTLN(analogRead(PIN_POWER_INDICATOR));*/ DEBUG_PRINT(lastPowerState); DEBUG_PRINT(F(" ")); DEBUG_PRINT(isPowerOK()); 
    setErrorFlag(badPower);
    
    if (!lastPowerState && isPowerOK()) {
      sendText(powerRestored);
      DEBUG_PRINTLN(F("Power Restored"));
    }
    if (lastPowerState && !isPowerOK()) {
      setFonaOff();
      DEBUG_PRINTLN(F("POWER FALL"));
    }
    
    
    DEBUG_PRINT(lastPowerState); DEBUG_PRINT(F(" ")); DEBUG_PRINTLN(isPowerOK());
  }
  lastPowerState = isPowerOK();
  
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
