 #include "Adafruit_FONA.h"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"

char *messageText;
unsigned long timestamp;
unsigned long errorDelay;


//char sendTo[8][16] = {"7202449051", "2246160041"}; // How to send to two numbers => Loop that uses a different char every time?

// make message for when profile is changed
const char *messages[] = {
  "WARNING: Temperature out of range: (rangeMin - rangeMax) Current Temp: %d",
  "WARNING: Power outage. Current Battery %: %d",
  "",
  "WARNING: Battery Low. Current Battery %: %d",
  "",
  "", 
  "WARNING: Thermister not detected.",
  "Periodic Report: Range is %d to %d. Current Temperature is %d",
  "POWER RESTORED: Current Battery % is %d", "Device has been reset",
  ""
};

void sendText() {
   //char *sendTo = "";
	 // send an SMS!
   static int counter = 0;
   counter++;
   if (counter ==  1)
    fona.sendSMS("7202449051", messageText); //send to => phone numbers
   else if (counter == 1000)
    counter = 0;
}

void chooseMessage(int eventNum) {
  switch(eventNum){
    case 0:
      sprintf(messageText, messages[0]);
      break;
    case 1:
      sprintf(messageText, messages[1], getBatteryPercentage());
      break;
    case 3:
      sprintf(messageText, messages[3], getBatteryPercentage());
      break;
   /* case 4:
      sprintf(messageText, messages[0]);
      break;
    case 5:
      sprintf(messageText, messages[0]);
      break;*/
    case 6:
      sprintf(messageText, messages[6]);
      break;
    case 7:
      sprintf(messageText, messages[7]);
      break;
    case 8:
      sprintf(messageText, messages[8], getBatteryPercentage());
      break;
    case 9:
      sprintf(messageText, messages[9]);
      break;
  }
}
