#include "../inc/MessageSender.hpp"

#define FONA_PS A3

static char errorStatus[32];
static char messageText[32];
//static char sendTo[8][16] = {"7202449051", "2246160041"} How to send to two numbers => Loop that uses a different char every time?

enum{
  char badTemp = 0; //will need return of temperature compare function to activate
  char badPower; //needs ps pin, do we initialize that or what
  char badSMS; // easy, use code from the test example
  char lowBattery; //easy, use code form the test example
  char fullSDCard; //check to see if this is possible + how to do  => seems possible using a header file => look here https://forum.arduino.cc/index.php?topic=139852.0
  //char noSDCard; // if port is empty? look into => I think its possible, should be a pin on the breakoutboard that I can use to do this
  char noThermister; //check to see how to do this => initial thought is seeing what happens when the thermister is disconeected and look for that 
}errors;

enum{
  periodicReport = 0;
  powerRestored;
  deviceReset;
} events;

void sendText() {
	 // send an SMS!
   fona.sendSMS(sendTo, messageText) //send to => phone numbers
   break;
}

void chooseMessage() {
	// Look at fizzbuzz video to add text to existing string efficiently => not exactly what i want
 uint16_t vbat;
 if (temperatureCompare == FALSE){
  messageText = "WARNING: Temperature out of range: (rangeMin - rangeMax) Current Temp: %d";
 }
 if (FONA_PS == 0){
  messageText = "WARNING: Power outage. Current Battery %: %d";
 }

  
 
}

void operation() {
	// Can't remember why i thought i needed this function => use to activate errorLEDs?
}
