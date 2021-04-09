#include "inc/Errors.h"
#include "inc/HWIO.hpp"

#define BLINK_DELAY (500)

char16_t errorFlag = 0;

void setErrorFlag(int eventNum){
  errorFlag |= bit(eventNum);
}

void blinkLED() {
  static uint32_t lastCommandMillis = 0;
  static bool currentState = false;

  long ms = millis();
  
  if ((errorFlag | badSMS) && ((lastCommandMillis > lastCommandMillis+BLINK_DELAY) && (ms >= lastCommandMillis))) {
    currentState = !currentState;
    if (currentState == false) setErrorLEDOff();
    else                       setErrorLEDOn();
    lastCommandMillis = ms + BLINK_DELAY;
  }
  else if (errorFlag != 0) {
    setErrorLEDOn();
  }
  else {
    setErrorLEDOff();
  }
}
