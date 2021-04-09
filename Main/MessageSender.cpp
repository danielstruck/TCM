#include "Adafruit_FONA.h"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"
#include "inc/Errors.h"
#include "inc/TemperatureDetection.hpp"
#include "inc/Profile.hpp"
#include "inc/Logger.hpp"

uint32_t lastSent = fifteenMinutes;
//uint32_t nextSent = 0;
uint32_t lastPeriodic = twentyfourHours;
//uint32_t nextPeriodic = 0;

// make message for when profile is changed
const char *messages[] = {
  "T",//"BAD TEMP",// range %d to %d current %d",
  "O",//"PWR OUT",// battery %d %%",
  "",
  "",
  "",
  "",
  "",//"Thermister not detected",
  "P",//"PERIODIC",// range %d to %d current %d",
  "R",//"PWR RESTORE",// battery: %d %%",
  "E",//"RST ERRS",
  "S",//"PROF SW"// range %d to %d current %d"
};

const char *msg_range() {
  char *msg;
  sprintf(msg, " range %d,%d", profile[currentProfile].lower, profile[currentProfile].upper);
  return msg;
}
const char *msg_current() {
  char *msg;
  sprintf(msg, " current %dC", temperatureChamber);
  return msg;
}
const char *msg_battery() {
  char *msg;
  sprintf(msg, " battery %d%%", getBatteryPercentage());
  return msg;
}

void sendError() {
  uint32_t currentTime = millis();
  String messageText = "WARNING";

  messageText += convertMillis(currentTime);
  
  if ((lastSent + fifteenMinutes < lastSent) && (currentTime >= lastSent)) {}
  else if (currentTime >= lastSent + fifteenMinutes) {
    for (int i = 0; i < 7; i++) {
      if (errorFlag & bit(i)) {
        messageText += "\n";
        messageText += messages[i];
        if (i == 0 || i == 7 || i ==10) {
          messageText += msg_range();
        }
        if (i == 0 || i == 7 || i == 10) {
          messageText += msg_current();
        }
        if (i == 1 || i == 8) {
          messageText += msg_battery();
        }
      }
    }
    if (!isFonaPowered)
      setFonaOn();
    long start = millis();
    fona.sendSMS("7202449051", messageText.c_str()); //send to => phone numbers
    long end_ = millis();
    DEBUG_PRINTLN(start); DEBUG_PRINTLN(end_);
    if (!isFonaPowered)
      setFonaOff();
    // fona.sendSMS("2246160041", messageText); //send to => phone numbers
    DEBUG_PRINTLN(F("Send Message"));
//    lastSent = currentTime;
//    nextSent = currentTime + fifteenMinutes;
    lastSent = currentTime + fifteenMinutes;
  }
}

void sendText(int eventNum) {
  uint32_t currentTime = millis();
  char* messageText;
//  DEBUG_PRINTLN(F("Send text function"));

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
      if ((lastPeriodic + twentyfourHours < lastPeriodic) && (currentTime >= lastPeriodic)) {
        //does nothing
      }
      else if (currentTime >= lastPeriodic) {
        sprintf(messageText, messages[eventNum], profile[currentProfile].lower, profile[currentProfile].upper, temperatureChamber);
        fona.sendSMS("7202449051", messageText);
//        lastPeriodic = currentTime;
//        nextPeriodic = currentTime + twentyfourHours;
        lastPeriodic = currentTime + twentyfourHours;
      }
  }
}
