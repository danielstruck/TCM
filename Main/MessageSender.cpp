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

char *recipients[5] = {
  "7202449051",
  "2246160041",
  "7138780209",
  "9283505990",
  ""
};
int registeredRecipients = 4;
int8_t nextSMSRecipient = -1;
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
    DEBUG_PRINT(F(" send message ")); DEBUG_PRINTLN(messageText.c_str()); 
    
    if (!isFonaPowered()) setFonaOn();
    for (int i = 0; i < 10 && !fona.sendSMS(recipients[nextSMSRecipient], messageText.c_str()); ++i) {
      /* wait for previous SMS to finish sending */
      delay(10);
    }
      
    nextSMSRecipient--;
  }
  else if (errorFlag && isFonaPowered()) {
    setFonaOff();
  }
}
void startSendingSMS() {
  nextSMSRecipient = registeredRecipients-1;
}
void sendError() { 
//  DEBUG_PRINT(F("sendError: ")); DEBUG_PRINTLN(errorFlag);
  uint32_t currentTime = millis();

  bool nextSendHasOverflowed = lastSent + fifteenMinutes < lastSent;
  bool currentTimeHasOverflowed = currentTime < lastSent;
  bool shouldSendSMS = lastSent == 0 || (currentTime >= lastSent + fifteenMinutes);
  DEBUG_PRINT(F("SMS countdown: ")); DEBUG_PRINTLN(lastSent + fifteenMinutes - currentTime);
  if (nextSendHasOverflowed && !currentTimeHasOverflowed) {
    /* wait for current time to overflow as well */
  }
  else if (shouldSendSMS) {
    messageText = currentTime;
    messageText += " WARNING: ";
    
    for (int i = 0; i < 7; i++) {
      if (errorFlag & bit(i)) {
        messageText += "\n";
        messageText += messages[i];
        if (i == 0 || i == 7 || i ==10) {
          DEBUG_PRINTLN(F("append range")); 
          messageText += " prof ";
          messageText += (int) profile[currentProfile].lower;
          messageText += ',';
          messageText += (int) profile[currentProfile].upper;
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
    
    lastSent = currentTime;
    startSendingSMS();
  }
}

void sendText(int eventNum) {
//  DEBUG_PRINT(F("sendText: ")); DEBUG_PRINTLN(eventNum);
  uint32_t currentTime = millis();

  if (eventNum == periodicReport) {
    if ((lastPeriodic + twentyfourHours < lastPeriodic) && (currentTime >= lastPeriodic)) {
      //does nothing
    }
    else if (currentTime >= lastPeriodic) {
      messageText = messages[eventNum];
      messageText += ' ';
      messageText += (int) profile[currentProfile].lower;
      messageText += ' ';
      messageText += (int) profile[currentProfile].upper;
      messageText += ' ';
      messageText += temperatureChamber;
      lastPeriodic = currentTime + twentyfourHours;
      
      startSendingSMS();
    }
  }
  else {
    messageText = messages[eventNum];
    
    switch (eventNum) {
      case powerRestored:
          messageText += ' ';
          messageText += getBatteryPercentage();
        break;
      case profileSwitched:
          messageText += ' ';
          messageText += (int) profile[currentProfile].lower;
          messageText += ',';
          messageText += (int) profile[currentProfile].upper;
          messageText += '|';
          messageText += temperatureChamber;
        break;
      case deviceReset:
        break;
    }
  
    startSendingSMS();
  }
}

void sms_resetError() {
  lastSent = 0;
}
