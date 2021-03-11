#include "Adafruit_FONA.h"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"

char *messageText;
long timestamp;
long errorDelay;

//char sendTo[8][16] = {"7202449051", "2246160041"}; // How to send to two numbers => Loop that uses a different char every time?

enum events{
  badTemp = 0, //will need return of temperature compare function to activate
  badPower, //needs ps pin, do we initialize that or what
  badSMS, // easy, use code from the test example
  lowBattery, //easy, use code form the test example
  fullSDCard, //check to see if this is possible + how to do  => seems possible using a header file => look here https://forum.arduino.cc/index.php?topic=139852.0 => if not doing sd card detect why do this
  noSDCard, // if port is empty? look into => I think its possible, should be a pin on the breakoutboard that I can use to do this
  noThermister //check to see how to do this => initial thought is seeing what happens when the thermister is disconeected and look for that => look for null on the port
  periodicReport,
  powerRestored,
  deviceReset,
};

static char* messages[10] = {"WARNING: Temperature out of range: (rangeMin - rangeMax) Current Temp: %d", "WARNING: Power outage. Current Battery %: %d", , "WARNING: Battery Low. Current Battery %: %d", , , 
                              "WARNING: Thermister not detected.", "Periodic Report: Range is %d to %d. Current Temperature is %d", "POWER RESTORED: Current Battery % is %d", "Device has been reset"
};

void sendText() {
   //char *sendTo = "";
	 // send an SMS!
   fona.sendSMS(7202449051, messageText); //send to => phone numbers
}

void chooseMessage(int eventNum) {
  switch(eventNum){
    case 0:
      sprintf(messageText, messages(0));
    case 1:
      sprintf(messageText, messages(1), getBatteryPercentage());
    case 3:
      sprintf(messageText, messages(3)getBatteryPercentage());
   /* case 4:
      sprintf(messageText, messages(0));
    case 5:
      sprintf(messageText, messages(0));*/
    case 6:
      sprintf(messageText, messages(6));
    case 7:
      sprintf(messageText, messages(7));
    case 8:
      sprintf(messageText, messages(8), getBatteryPercentage());
    case 9:
      sprintf(messageText, messages(9));
  }
}

void blinkLED(int eventNum) {
	// Error LED Sets
 
  for (int i = 0; i < 3; i++){
    int bit = (eventNum >> i) & 1;

    if (bit == 0)
      blinkShort();
    else 
      blinkLong();
 
 }

 delay(5000);
 
}

void blinkShort(){
  setErrorLEDOn();
  delay(500);
  setErrorLEDOff();
  delay(500);
}

void blinkLong() {
  setErrorLEDOn();
  delay(1000);
  setErrorLEDOff();
  delay(500);
}
