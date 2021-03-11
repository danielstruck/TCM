#include "Adafruit_FONA.h"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"

char errorStatus[32];
char *messageText;
char sendTo[8][16] = {"7202449051", "2246160041"}; // How to send to two numbers => Loop that uses a different char every time?

enum errors{
  badTemp = 0, //will need return of temperature compare function to activate
  badPower, //needs ps pin, do we initialize that or what
  badSMS, // easy, use code from the test example
  lowBattery, //easy, use code form the test example
  //fullSDCard, //check to see if this is possible + how to do  => seems possible using a header file => look here https://forum.arduino.cc/index.php?topic=139852.0 => if not doing sd card detect why do this
  //noSDCard, // if port is empty? look into => I think its possible, should be a pin on the breakoutboard that I can use to do this
  noThermister //check to see how to do this => initial thought is seeing what happens when the thermister is disconeected and look for that => look for null on the port
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

void chooseMessage(char code) {
  //uint16_t vbat;
 if (code == "badTemp"){
  messageText = "WARNING: Temperature out of range: (rangeMin - rangeMax) Current Temp: %d";
 }
  if (code == "badPower"){
    messageText = "WARNING: Power outage. Current Battery %: %d";
  }
  if (code == "lowBattery"){
    messageText = "WARNING: Battery Low. Current Battery %: %d";
  }
  if (code == "nominal"){
    messageText = "Periodic Report: Range is %d to %d. Current Temperature is %d";
  }
  if (code == "powerRestored"){
    messageText = "POWER RESTORED: Current Battery % is %d";
  }
  if (code == "deviceReset"){
    messageText = "Device has been reset";
  }
}

void blinkLED(int code) {
	// Error LED Sets
 uint8_t short = 100;
 uint8_t long = 1000;
 if (code == "badTemp"){
  convertToBinary(badTemp);
 }
 
}

/*char convertToBinary(int numToConvert){
 char binary;
 while(numToConvert != 0){
  binary = (numToConvert % 2 == 0 ? "0" : "1") + binary;
  numToConvert /= 2;
 }
 return binary; 
}*/
