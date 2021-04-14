#ifndef _MESSAGE_SENDER_HPP_
#define _MESSAGE_SENDER_HPP_


// (4,294,967,295); //Max value of millis()
#define time_Max (4294967295)
// (900,000); //Fifteen minutes worth of milliseconds
#define fifteenMinutes (900000)
#define twentyfourHours (86400000)


//extern char messageText[128];
//extern char sendTo[8][16]; // How to send to two numbers => Loop that uses a different char every time?
//extern unsigned long timestamp;
//extern const char *messages[];

void sendText(int eventNum);

void sendError();

void sendMessageToNextRecipient();

//char* chooseMessage(int eventNum);

#endif // _MESSAGE_SENDER_HPP_
