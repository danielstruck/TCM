#include "inc/Errors.h"
#include "inc/HWIO.hpp"
#include <ctype.h>

char16_t errorFlag = 0;

void setErrorFlag(int eventNum){
  //for (int i = 0; i < 3; i++)
     // int bit = (errorFlag >> i) & 1;
    errorFlag |= bit(eventNum);
     
}

void blinkLED(int eventNum) {
  // Error LED Sets
 
  for (int i = 0; i < 3; i++){
   // int bit = (eventNum >> i) & 1;

  int currentBit = bit(i) & eventNum;

    if (currentBit == 0)
      blinkShort();
    else 
      blinkLong();
 
 }

 delay(5000);
 
}

void blinkShort(){
  Serial.println("blink short");
  setErrorLEDOn();
  delay(500);
  setErrorLEDOff();
  delay(500);
}

void blinkLong() {
  Serial.println("blink long");
  setErrorLEDOn();
  delay(1000);
  setErrorLEDOff();
  delay(500);
}
