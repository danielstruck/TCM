#ifndef _MESSAGE_SENDER_HPP_
#define _MESSAGE_SENDER_HPP_



extern char *messageText;
//extern char sendTo[8][16]; // How to send to two numbers => Loop that uses a different char every time?
extern long timestamp;
extern long errorDelay;
extern static char* [10];

extern enum events;


void sendText();

void chooseMessage();

void blinkLED();

void blinkShort();

void blinkLong();

#endif // _MESSAGE_SENDER_HPP_