#ifndef _MESSAGE_SENDER_HPP_
#define _MESSAGE_SENDER_HPP_


extern char errorStatus[32];
extern char *messageText;
extern char sendTo[8][16]; // How to send to two numbers => Loop that uses a different char every time?


void sendText();

void chooseMessage();

void operation();

#endif // _MESSAGE_SENDER_HPP_