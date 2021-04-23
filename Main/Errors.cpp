#include "inc/Errors.h"
#include "inc/HWIO.hpp"

#define BLINK_DELAY (500)

// contains a list of errors that have been thrown which can be accessed
//  by  {errorFlag} | bit({eventNum})
char16_t errorFlag = 0;

// sets the error flag associated with {eventNum} from the {events} enum in Errors.h
void setErrorFlag(int eventNum){
  errorFlag |= bit(eventNum);
}

// if can't connect to Fona or 3G network  ->  blinks the error LED with a period of 1 second
// if off-nominal error exists  ->  sets the error LED on
// otherwise  ->  sets the error LED off
void blinkLED() {
  static uint32_t lastCommandMillis = 0;
  static bool currentState = false;

  long ms = millis();
  
  if (errorFlag | badSMS) {
    // determines if 500 milliseconds have passed then toggled the state of the error LED
    if ((lastCommandMillis > lastCommandMillis+BLINK_DELAY) && (ms >= lastCommandMillis)) {
      currentState = !currentState;
      if (currentState == false) setErrorLEDOff();
      else                       setErrorLEDOn();
      lastCommandMillis = ms + BLINK_DELAY;
    }
  }
  else if (errorFlag != 0) {
    setErrorLEDOn();
  }
  else {
    setErrorLEDOff();
  }
}
