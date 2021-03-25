#include "Adafruit_FONA.h"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"
#include "inc/Errors.h"

// (4,294,967,295); //Max value of millis()
#define time_Max (4294967295)
// (900,000); //Fifteen minutes worth of milliseconds
#define fifteenMinutes (900000)

char messageText[128];
unsigned long timestamp;
unsigned long errorDelay;
unsigned long lastSentArray[10];
unsigned long nextSentArray[10];


//char sendTo[8][16] = {"7202449051", "2246160041"}; // How to send to two numbers => Loop that uses a different char every time?

// make message for when profile is changed
const char *messages[] = {
  "WARNING: Temperature out of range: (rangeMin - rangeMax) Current Temp: %d",
  "WARNING: Power outage. Current Battery %%: %d",
  "",
  "",//"WARNING: Battery Low. Current Battery %%: %d",
  "",
  "", 
  "WARNING: Thermister not detected.",
  "Periodic Report: Range is %d to %d. Current Temperature is %d",
  "POWER RESTORED: Current Battery %% is %d", 
  "Device reset button pressed. Restarting operations",
  ""
};

void sendText(int eventNum) {
  Serial.println("Send text function");
   //char *sendTo = "";
	 // send an SMS!   
   
   chooseMessage(eventNum);
   millisOverflow(eventNum);

   unsigned long ms = millis();
   
   if (ms >= nextSentArray[eventNum]) {
      fona.sendSMS("7202449051", messageText); //send to => phone numbers
     // fona.sendSMS("2246160041", messageText); //send to => phone numbers
      Serial.println("Send Message");
      lastSentArray[eventNum] = ms;
      nextSentArray[eventNum] = ms + fifteenMinutes;
   }
}

void chooseMessage(int eventNum) {
  Serial.println("choose message function");
  switch(eventNum){
    case badTemp:
      sprintf(messageText, messages[0]);
      break;
    case badPower:
      sprintf(messageText, messages[1], getBatteryPercentage());
      Serial.println("Bad power set");
      break;
   /* case lowBattery:
      sprintf(messageText, messages[3], getBatteryPercentage());
      break;
      case 4:
      sprintf(messageText, messages[0]);
      break;
    case 5:
      sprintf(messageText, messages[0]);
      break;*/
    case noThermister:
      sprintf(messageText, messages[6]);
      break;
    case periodicReport:
      sprintf(messageText, messages[7]);
      break;
    case powerRestored:
      sprintf(messageText, messages[8], getBatteryPercentage());
      break;
    case deviceReset:
      sprintf(messageText, messages[9]);
      break;
  }

  Serial.print("Message: "); Serial.println(messageText);
}

 void millisOverflow(int eventNum){
   unsigned long ms = millis();

   if (ms < lastSentArray[eventNum])
     nextSentArray[eventNum] -= time_Max;
  }
