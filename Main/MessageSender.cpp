#include "Adafruit_FONA.h"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"
#include "inc/Errors.h"
#include "inc/TemperatureDetection.hpp"
#include "inc/Profile.hpp"
#include "inc/Logger.hpp"
#include "inc/Main.hpp"

uint32_t lastSent = 0;
uint32_t lastPeriodic = twentyfourHours;

// Contains message templates for all implemented.
// Currently only single characters as the device does not have enough RAM
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

// contains phone numbers of recipients
char *recipients[5] = {
  "7202449051",
  "2246160041",
  "7138780209",
  "9283505990",
  ""
};
// keeps track of the number of registered recipients
int registeredRecipients = 4; 
// keeps track of the next recipient to send the message to; -1 means that it is idle and not sending messages
int8_t nextSMSRecipient = -1;
// contains the message string
String messageText;

// returns false if array is full, true if number was added
// 04-23-2021  Unused, but should append new recipients to the array
bool registerNewRecipient(char *phoneNumber) {
  if (registeredRecipients == 5) 
    return false;
  recipients[registeredRecipients] = phoneNumber;
  registeredRecipients++;
  return true;
}

// sends a single SMS notification to the next recipient (if any), which is determined by
//  the variable nextSMSRecipient. If nextSMSRecipient is a valid index of the recipients 
//  array (effectivelly if >= 0) then attempt to send the message at most 10 times before
//  timing out. Also handles turning the Fona on and off.
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

// tells the system to start sending SMS messages
void startSendingSMS() {
  nextSMSRecipient = registeredRecipients-1;
}

// Builds messages to send to recipients for off-nominal events
void sendError() { 
  // Collect the current time now to prevent potential overflows from breaking our checks
  uint32_t currentTime = millis();

  // alias several conditional statements
  bool nextSendHasOverflowed = lastSent + fifteenMinutes < lastSent;
  bool currentTimeHasOverflowed = currentTime < lastSent;
  bool shouldSendSMS = lastSent == 0 || (currentTime >= lastSent + fifteenMinutes);
  
  // print out the time until the next error message should be sent
  //DEBUG_PRINT(F("SMS countdown: ")); DEBUG_PRINTLN(lastSent + fifteenMinutes - currentTime);

  // handle overflow conditions
  if (nextSendHasOverflowed && !currentTimeHasOverflowed) {
    /* wait for current time to overflow as well */
  }
  // build the message (or rebuild it if it already exists)
  //  and tell the system to start sending messages
  else if (shouldSendSMS) {
    // reset the message text in the format:
    //     {timestamp in milliseconds} WARNING: [{newline}{message}...]
    messageText = currentTime;
    messageText += " WARNING: ";
    
    for (int i = 0; i < 7; i++) {
      // check each off-nominal error flag, if on then append its message
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

// Builds messages to send to recipients for events that are not the off-nominal events
//  04-23-2021  handles: periodicReport, powerRestored, deviceReset, profileSwitched  ref Errors.h
void sendText(int eventNum) {
  // Collect the current time now to prevent potential overflows from breaking our checks
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
    // select the event's message as the message to send
    messageText = messages[eventNum];

    // append additional information on the system state depending on the event
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

    // tell the system that it should start sending messages
    startSendingSMS();
  }
}

// Called when reset button is pressed to reset the error message sending.
// This gives the system the ability to send errors immediately after a 
//  reset occurs. If the system sends an SMS right before a reset, then 
//  the system would not be able to send an SMS until some time after the
//  one that was just sent.
void sms_resetError() {
  lastSent = 0;
}
