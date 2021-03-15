#ifndef _MESSAGE_SENDER_HPP_
#define _MESSAGE_SENDER_HPP_



extern char *messageText;
//extern char sendTo[8][16]; // How to send to two numbers => Loop that uses a different char every time?
extern unsigned long timestamp;
extern unsigned long errorDelay;
extern const char *messages[];

enum events{
  badTemp = 0, //will need return of temperature compare function to activate
  badPower, //needs ps pin, do we initialize that or what
  badSMS, // easy, use code from the test example
  lowBattery, //easy, use code form the test example
  fullSDCard, //check to see if this is possible + how to do  => seems possible using a header file => look here https://forum.arduino.cc/index.php?topic=139852.0 => if not doing sd card detect why do this
  noSDCard, // if port is empty? look into => I think its possible, should be a pin on the breakoutboard that I can use to do this
  noThermister, //check to see how to do this => initial thought is seeing what happens when the thermister is disconeected and look for that => look for null on the port
  periodicReport,
  powerRestored,
  deviceReset,
};


void sendText();

void chooseMessage(int eventNum);

void blinkLED(int eventNum);

void blinkShort();

void blinkLong();

#endif // _MESSAGE_SENDER_HPP_