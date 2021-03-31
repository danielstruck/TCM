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

  // enable and start up Fona 3G
  setLEDs(0, 1, 0, 0, 0);
  setupFona();

  // setup logger and SD card bus
  setLEDs(0, 1, 1, 0, 0);
  setupLogger();

  // ask user to input initial profile parameters
  setupInitialProfile();

  // turn off error LED
  setLEDs(0, -1, -1, -1, -1);
  
  
  Serial.println("Setup complete");
}

void loop() {
  char stateStr[128];
  
//  uint8_t rtc_year;
//  uint8_t rtc_month;
//  uint8_t rtc_day;
//  uint8_t rtc_hour;
//  uint8_t rtc_minute;
//  uint8_t rtc_second; 
//  uint8_t rtc_tz;
//  fona.readRTC(&rtc_year, &rtc_month, &rtc_day, &rtc_hour, &rtc_minute, &rtc_second, &rtc_tz);
//  sprintf(stateStr, "err=%-3d  tmp=%-6d  pwrOK=%-2d  rst=%-2d  rtc=%02d/%02d/%04d %02d:%02d:%02d+%d",
//          errorFlag, isTemperatureInsideBoundries(), isPowerOK(), resetBtnPressed(),
//          rtc_month, rtc_day, rtc_year, rtc_hour, rtc_minute, rtc_second, rtc_tz);

//  char timeStr[64];
//  getTime(timeStr, 64);
//  sprintf(stateStr, "err=%-3d  tmp=%-6d  pwrOK=%-2d  rst=%-2d  tim=%s",
//          errorFlag, isTemperatureInsideBoundries(), isPowerOK(), resetBtnPressed(), timeStr);
//  Serial.println(stateStr);

  sprintf(stateStr, "err=%-3d  prof=%d  tmp=%-6d  pwrOK=%-2d  rst=%-2d",
          errorFlag, currentProfile, isTemperatureInsideBoundries(), isPowerOK(), resetBtnPressed());
  Serial.println(stateStr);
  
  
  logData(temperatureChamber);
  
  // check termal range
  temp_sense();
//  if (!isTemperatureInsideBoundries() || (errorFlag & bit(badTemp))) {
//    Serial.println("> Bad temperature detected");
//    sendSMSWithError(badTemp);
//  }
//
//  if (!isPowerOK() || (errorFlag & bit(badPower))) {
//    Serial.print("> Power outage detected "); Serial.println(analogRead(PIN_POWER_INDICATOR));
//    sendSMSWithError(badPower);
//    if (isPowerOK())
//      sendText(powerRestored);
//  }

  
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
