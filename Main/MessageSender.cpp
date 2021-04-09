#include "Adafruit_FONA.h"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"
#include "inc/Errors.h"
#include "inc/TemperatureDetection.hpp"
#include "inc/Profile.hpp"
#include "inc/Logger.hpp"

uint32_t lastSent = 0;
uint32_t nextSent = 0;
uint32_t lastPeriodic = 0;
uint32_t nextPeriodic = 0;

// make message for when profile is changed
const char *messages[] = {
  "Temperature out of range %d to %d, current: %d",
  "POWER OUTAGE battery = %d %%",
  "",
  "",
  "",
  "",
  "Thermister not detected",
  "PERIODIC REPORT range %d to %d, Current: %d",
  "POWER RESTORED battery: %d %%",
  "RESET PRESSED restarting operations",
  "PROFILE SWITCHED range %d to %d, current: %d"
};

void sendError() {
  uint32_t currentTime = millis();
  String messageText = "WARNING";

  messageText += convertMillis(currentTime);
  
  if ((nextSent < lastSent) && (currentTime >= lastSent)) {}
  else if (currentTime >= nextSent) {
    for (int i = 0; i < 7; i++) {
      if (errorFlag & bit(i)) {}
      messageText += "\n";
      messageText += messages[i];
    }
    if (!isFonaPowered)
      setFonaOn();
    fona.sendSMS("7202449051", messageText.c_str()); //send to => phone numbers
    
    setFonaOff();
    // fona.sendSMS("2246160041", messageText); //send to => phone numbers
    DEBUG_PRINTLN(F("Send Message"));
    lastSent = currentTime;
    nextSent = currentTime + fifteenMinutes;
  }
}

void sendText(int eventNum) {
  uint32_t currentTime = millis();
  char* messageText;
  DEBUG_PRINTLN(F("Send text function"));

  switch (eventNum) {
    case powerRestored:
      sprintf(messageText, messages[eventNum], getBatteryPercentage());
      fona.sendSMS("7202449051", messageText);
      break;
    case profileSwitched:
      sprintf(messageText, messages[eventNum], profile[currentProfile].lower, profile[currentProfile].upper, temperatureChamber);
      fona.sendSMS("7202449051", messageText);
      break;
    case deviceReset:
      sprintf(messageText, messages[eventNum]);
      fona.sendSMS("7202449051", messageText);
      break;
    case periodicReport:
      if ((nextPeriodic < lastPeriodic) && (currentTime >= lastPeriodic)) {
        //does nothing
      }
      else if (currentTime >= lastPeriodic) {
        sprintf(messageText, messages[eventNum], profile[currentProfile].lower, profile[currentProfile].upper, temperatureChamber);
        fona.sendSMS("7202449051", messageText);
        lastPeriodic = currentTime;
        nextPeriodic = currentTime + twentyfourHours;
      }
  }
}
