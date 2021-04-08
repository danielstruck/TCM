#include "Adafruit_FONA.h"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"
#include "inc/Errors.h"
#include "inc/TemperatureDetection.hpp"
#include "inc/Profile.hpp"

uint32_t lastSent = 0;
uint32_t nextSent = 0;
uint32_t lastPeriodic = 0;
uint32_t nextPeriodic = 0;

// make message for when profile is changed
const char *messages[] = {
  "WARNING: Temperature out of range: (rangeMin - rangeMax) Current Temp: %d",
  "WARNING: Power outage. Current Battery %%: %d",
//  "",
//  "",
//  "",
//  "",
  "WARNING: Thermister not detected.",
  "Periodic Report: Range is %d to %d. Current Temperature is %d",
  "POWER RESTORED: Current Battery %% is %d",
  "Device reset button pressed. Restarting operations",
  "Profile Switched: Range is %d to %d. Current Temperature is %d"
};

char* chooseMessage(int eventNum) {
	char messageText[128]; // !!! potential resource leak - move to its own function to fix

	DEBUG_PRINTLN(F("choose message function"));
	switch (eventNum) {
//	case badTemp:
//		sprintf(messageText, messages[0], temperatureChamber);
//		break;
//	case badPower:
//		sprintf(messageText, messages[1], getBatteryPercentage());
//		DEBUG_PRINTLN(F("Bad power set"));
//		break;
//		/* case lowBattery:
//		   sprintf(messageText, messages[3], getBatteryPercentage());
//		   break;
//		   case 4:
//		   sprintf(messageText, messages[0]);
//		   break;
//		 case 5:
//		   sprintf(messageText, messages[0]);
//		   break;*/
//	case noThermister:
//		sprintf(messageText, messages[6]);
//		break;
	case periodicReport:
		sprintf(messageText, messages[7], profile[currentProfile].lower, profile[currentProfile].upper, temperatureChamber);
		break;
	case powerRestored:
		sprintf(messageText, messages[8], getBatteryPercentage());
		break;
	case deviceReset:
		sprintf(messageText, messages[9]);
		break;
	}

	DEBUG_PRINT(F("Message: ")); DEBUG_PRINTLN(messageText);
	return messageText;
}

void sendError(){
  String messageText ="WARNING";
  uint32_t currentTime = millis();

  if ((nextSent < lastSent) && (currentTime >= lastSent)) {}
  else if (currentTime >= nextSent) {
    for (int i = 0; i < 7; i++){
      if (errorFlag & bit(i)){}
        messageText += "\n";
        messageText += messages[i];
    }
--    if (!isFonaPowered)
--      setFonaOn();
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
  bool shouldSend = true;
  DEBUG_PRINTLN(F("Send text function"));

  switch (eventNum) {
    case powerRestored:
      sprintf(messageText, messages[eventNum], getBatteryPercentage());
      break;
    case profileSwitched:
      sprintf(messageText, messages[eventNum], profile[currentProfile].lower, profile[currentProfile].upper, temperatureChamber);
      break;
    case deviceReset:
      sprintf(messageText, messages[eventNum]);
      break;
    case periodicReport:
      if ((nextPeriodic < lastPeriodic) && (currentTime >= lastPeriodic)) {
        //does nothing
        shouldSend = false;
      }
      else if (currentTime >= lastPeriodic) {
        sprintf(messageText, messages[eventNum], profile[currentProfile].lower, profile[currentProfile].upper, temperatureChamber);
        lastPeriodic = currentTime;
        nextPeriodic = currentTime + twentyfourHours;
      }
  }
  if (shouldSend)
    fona.sendSMS("7202449051", messageText);
}
