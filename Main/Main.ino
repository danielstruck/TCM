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
  setupPins(); // must go before Fona and Logger setups or the pins will not be set up correctly
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

// prints out the state of the system to Serial using Arduino's FlashStrings
void printState() {
  DEBUG_PRINT(F("err=")); DEBUG_PRINT(errorFlag);
  DEBUG_PRINT(F("\tprof=")); DEBUG_PRINT(currentProfile);
  DEBUG_PRINT(F("\ttemp=")); DEBUG_PRINT((int) temperatureChamber);
  DEBUG_PRINT(F("\tbound=")); DEBUG_PRINT((int) profile[currentProfile].lower); DEBUG_PRINT(F(","));DEBUG_PRINT((int) profile[currentProfile].upper);
  DEBUG_PRINT(F("\tpwrOk=")); DEBUG_PRINT(isPowerOK());
  DEBUG_PRINT(F("\trst=")); DEBUG_PRINT(resetBtnPressed());
  DEBUG_PRINTLN(F(""));
}

void loop() {
  static bool lastPowerState = true; // used to track last power state for power outage and power restored events

  // prints out the state of the system to Serial
  printState();
  
  // check termal range
  temp_sense();

  if (!isTemperatureInsideBoundries() || (errorFlag & bit(badTemp))) {
    DEBUG_PRINTLN(F("> Bad temperature detected"));
    setErrorFlag(badTemp);
    sendError();
  }

  // record temperature data on the microSD card
  logData(temperatureChamber);
      
  if (!isPowerOK() || (errorFlag & bit(badPower))) {
    setErrorFlag(badPower);
    sendError();

    // On a rising edge of power, send a power restored notification and turn the Fona back on
    if (!lastPowerState && isPowerOK()) {
      DEBUG_PRINTLN(F("Power Restored"));
      sendText(powerRestored);
      setFonaOn();
    }

    // On a falling edge of power, turn off the Fona
    if (lastPowerState && !isPowerOK()) {
      DEBUG_PRINTLN(F("POWER FALL"));
      setFonaOff();
    }
  }
  // update the last power state
  lastPowerState = isPowerOK();

  // reset errors if the reset button is pressed and errors exist
  if (resetBtnPressed() && errorFlag != 0) {
    DEBUG_PRINTLN(F("> Reset ON"));
    sendText(deviceReset);
    sms_resetError(); // resets the error message send time
    errorFlag = 0; // resets the error flags
  }

  // Try to send a periodic report if there are no errors in the system
  if (!errorFlag) {
    sendText(periodicReport);
  }
      
  // read button to change current profile
  if (isProfileBtnRising()) {
    DEBUG_PRINTLN(F("> Profile increment"));
    incrementProfile();
    sendText(profileSwitched);

    // update the profile LEDs
    setLEDs(-1, currentProfile == 0,
                currentProfile == 1,
                currentProfile == 2,
                currentProfile == 3);
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

  // tell the system to check for a next message recipient and send the message if they exist, see MessageSender
  sendMessageToNextRecipient();
}
