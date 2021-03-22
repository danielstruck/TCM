#include "Adafruit_FONA.h"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"
#include "inc/Errors.h"

char messageText[128];
unsigned long timestamp;
unsigned long errorDelay;


//char sendTo[8][16] = {"7202449051", "2246160041"}; // How to send to two numbers => Loop that uses a different char every time?

// make message for when profile is changed
const char *messages[] = {
  "WARNING: Temperature out of range: (rangeMin - rangeMax) Current Temp: %d",
  "WARNING: Power outage. Current Battery %%: %d",
  "",
  "WARNING: Battery Low. Current Battery %%: %d",
  "",
  "", 
  "WARNING: Thermister not detected.",
  "Periodic Report: Range is %d to %d. Current Temperature is %d",
  "POWER RESTORED: Current Battery %% is %d", "Device has been reset",
  ""
};

void sendText() {
  Serial.println("Send text function");
   //char *sendTo = "";
	 // send an SMS!
   static int counter = 0;
   counter++;
   if (counter ==  1) {
//    fona.sendSMS("7202449051", messageText); //send to => phone numbers
      fona.sendSMS("2246160041", messageText); //send to => phone numbers
      Serial.println("Send Message");
   }
   else if (counter == 5)
    counter = 0;

  char str[32]; sprintf(str, "counter = %d", counter);
   Serial.println(str);
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
    case lowBattery:
      sprintf(messageText, messages[3], getBatteryPercentage());
      break;
   /* case 4:
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
