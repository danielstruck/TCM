#include "Adafruit_FONA.h"

#define FONA_PS  (A3)
#define FONA_RST (4)

static char errorStatus[32];
static char *messageText;
//static char sendTo[8][16] = {"7202449051", "2246160041"} How to send to two numbers => Loop that uses a different char every time?
Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);

enum errors{
  badTemp = 0, //will need return of temperature compare function to activate
  badPower, //needs ps pin, do we initialize that or what
  badSMS, // easy, use code from the test example
  lowBattery, //easy, use code form the test example
  fullSDCard, //check to see if this is possible + how to do  => seems possible using a header file => look here https://forum.arduino.cc/index.php?topic=139852.0
  //noSDCard, // if port is empty? look into => I think its possible, should be a pin on the breakoutboard that I can use to do this
  noThermister //check to see how to do this => initial thought is seeing what happens when the thermister is disconeected and look for that 
};

enum{
  periodicReport = 0,
  powerRestored,
  deviceReset,
} events;

static char numbers[8][10] = {
  
};

void sendText() {
   char *sendTo = "";
	 // send an SMS!
   fona.sendSMS(sendTo, messageText); //send to => phone numbers
}

void chooseMessage() {
	// Look at fizzbuzz video to add text to existing string efficiently => not exactly what i want
 uint16_t vbat;
// if (temperatureInsideBoundries == FALSE){
//  messageText = "WARNING: Temperature out of range: (rangeMin - rangeMax) Current Temp: %d";
// }
 if (FONA_PS == 0){
  messageText = "WARNING: Power outage. Current Battery %: %d";
 }

  
 
}

void operation() {
	// TODO
}
