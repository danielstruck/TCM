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
  "@2",
  "@3",
  "@4",
  "@5",
  "WARNING: Thermister not detected.",
  "Periodic Report: Range is %d to %d. Current Temperature is %d",
  "POWER RESTORED: Current Battery %% is %d",
  "Device reset button pressed. Restarting operations",
  "Profile Switched: Range is %d to %d. Current Temperature is %d"
};

char* chooseMessage(int eventNum) {
	char messageText[128];

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
  char* messageText ="WARNING";
  uint32_t currentTime = millis();
  
  if ((nextSent < lastSent) && (currentTime >= lastSent)){}
  else if (currentTime >= nextSent) {
  for (int i = 0; i < 7; i++){
    if (errorFlag & bit(i))
      messageText += "\n";
      messageText += messages[i];
  }
  fona.sendSMS("7202449051", messageText); //send to => phone numbers
    // fona.sendSMS("2246160041", messageText); //send to => phone numbers
  DEBUG_PRINTLN(F("Send Message"));
  lastSent = currentTime;
  nextSent = currentTime + fifteenMinutes;
}

void sendText(int eventNum) {
	uint32_t currentTime = millis();
	char* messageText;
	DEBUG_PRINTLN(F("Send text function"));

	
  

//  if (eventNum == powerRestored)){//was power restored
//    messageText = chooseMessage(eventNum);
//    fona.sendSMS("7202449051", messageText);
//  }
//  else if (eventNum == profileSwitched){
//    messageText = chooseMessage(eventNum);
//    fona.sendSMS("7202449051", messageText);
//  }
//  else if (eventNum == periodicReport){
//    if ((nextPeriodic < lastPeriodic) && (currentTime >= lastPeriodic)){}
//    else if (currentTime >= lastPeriodic){
//      messageText = chooseMessage(eventNum);
//      fona.sendSMS("7202449051", messageText);
//      lastPeriodic = currentTime;
//      nextPeriodic = currentTime + twentyfourHours;
//    }
//  }
//	
//    messageText = chooseMessage(eventNum);
//		
               
		
		//      delay(5000); // delay serial print so we know the message has been sent 
	}
}
