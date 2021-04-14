#include "Adafruit_FONA.h"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"
#include "inc/Errors.h"
#include "inc/TemperatureDetection.hpp"
#include "inc/Profile.hpp"
#include "inc/Logger.hpp"

uint32_t lastSent = 0;
//uint32_t nextSent = 0;
uint32_t lastPeriodic = twentyfourHours;
//uint32_t nextPeriodic = 0;

// make message for when profile is changed
const char *messages[] = {
  "T",//"BAD TEMP",// range %d to %d current %d",
  "O",//"PWR OUT",// battery %d %%",
  "1",
  "2",
  "3",
  "4",
  "5",//"Thermister not detected",
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

char *recipients[5] = {
  "7202449051",
  "2246160041",
  "",
  "",
  ""
};
int registeredRecipients = 2;
uint8_t nextSMSRecipient = -1;
String messageText;
// returns false if array is full, true if number was added
int registerNewRecipient(char *phoneNumber) {
  if (registeredRecipients == 5) 
    return false;
  recipients[registeredRecipients] = phoneNumber;
  registeredRecipients++;
  return true;
}
void sendMessageToNextRecipient() {
  if (nextSMSRecipient >= 0) {
    DEBUG_PRINT(F("Send Message: ")); DEBUG_PRINTLN(messageText.c_str());
    
    if (!isFonaPowered()) setFonaOn();
    while (!fona.sendSMS(recipients[nextSMSRecipient], messageText.c_str()))
      /* wait for previous SMS to finish sending */
    if (isFonaPowered()) setFonaOff();
      
    nextSMSRecipient--;
  }
}
void sendError() {
//  DEBUG_PRINTLN(F("sendError")); 
  uint32_t currentTime = millis();
//  static String messageText = "WARNING ";
//  char *msg;

//  messageText += convertMillis(currentTime);

  bool nextSendHasOverflowed = lastSent + fifteenMinutes < lastSent;
  bool currentTimeHasOverflowed = currentTime < lastSent;
  bool shouldSendSMS = lastSent == 0 || (currentTime >= lastSent + fifteenMinutes);
  
  if (nextSendHasOverflowed && !currentTimeHasOverflowed) {
    /* wait for current time to overflow as well */
  }
  else if (shouldSendSMS) {
    messageText = currentTime;
    messageText += "WARNING: ";
    messageText += ' ';
    
    for (int i = 0; i < 7; i++) {
      if (errorFlag & bit(i)) {
        messageText += "\n";
        messageText += messages[i];
        if (i == 0 || i == 7 || i ==10) {
          DEBUG_PRINTLN(F("append range")); 
          messageText += " prof ";
          messageText += profile[currentProfile].lower;
          messageText += ',';
          messageText += profile[currentProfile].upper;
        }
        if (i == 0 || i == 7 || i == 10) {
          DEBUG_PRINTLN(F("append temp"));
          messageText += " temp ";
          messageText += temperatureChamber;
        }
        if (i == 1 || i == 8) {
          DEBUG_PRINTLN(F("append batt"));
          messageText += " batt ";
          messageText += getBatteryPercentage();
        }
      }
    }
    
    nextSMSRecipient = registeredRecipients-1;
    lastSent = currentTime + fifteenMinutes;
  }
}

void sendText(int eventNum) {
  uint32_t currentTime = millis();
//  static char* messageText;
//  DEBUG_PRINTLN(F("Send text function"));

  messageText = messages[eventNum];

  switch (eventNum) {
    case powerRestored:
//      sprintf(messageText, messages[eventNum], getBatteryPercentage());
        messageText += ' ';
        messageText += getBatteryPercentage();
      break;
    case profileSwitched:
//      sprintf(messageText, messages[eventNum], profile[currentProfile].lower, profile[currentProfile].upper, temperatureChamber);
        messageText += ' ';
        messageText += profile[currentProfile].lower;
        messageText += ' ';
        messageText += profile[currentProfile].upper;
        messageText += ' ';
        messageText += temperatureChamber;
      break;
    case deviceReset:
//      sprintf(messageText, messages[eventNum]);
      break;
    case periodicReport:
      if ((lastPeriodic + twentyfourHours < lastPeriodic) && (currentTime >= lastPeriodic)) {
        //does nothing
      }
      else if (currentTime >= lastPeriodic) {
//        sprintf(messageText, messages[eventNum], profile[currentProfile].lower, profile[currentProfile].upper, temperatureChamber);
//        lastPeriodic = currentTime;
//        nextPeriodic = currentTime + twentyfourHours;
        messageText += ' ';
        messageText += profile[currentProfile].lower;
        messageText += ' ';
        messageText += profile[currentProfile].upper;
        messageText += ' ';
        messageText += temperatureChamber;
        lastPeriodic = currentTime + twentyfourHours;
      }
  }

  nextSMSRecipient = registeredRecipients-1;
}
