#include "inc/HWIO.hpp"
#include "Adafruit_FONA.h"

Adafruit_FONA_3G fona = Adafruit_FONA_3G(PIN_FONA_RST);


// returns true if the profile button is pressed 
bool profileBtnPressed() {
  static int debounce = 0;
  
  if (digitalRead(PIN_BTN_SELECT) == HIGH && debounce < 100) {
    ++debounce;
  }
  else if (debounce > 0){
    --debounce;
  }
  
  return debounce == 100;
}

// returns true if the reset button is pressed
bool resetBtnPressed() {
  static int debounce = 0;
  
  if (digitalRead(PIN_BTN_RST) == HIGH && debounce < 100) {
    ++debounce;
  }
  else if (debounce > 0){
    --debounce;
  }
  
  return debounce == 100;
}

// returns true if the arduino is recieving power from the Fona
bool isPowerOK() {
  return digitalRead(PIN_POWER_INDICATOR) == HIGH;
}

// returns -1 on failure, current battery percentage otherwise
uint16_t getBatteryPercentage() {
  uint16_t vPer;
  if (fona.getBattPercent(&vPer)) {
    vPer = -1;
  }
  return vPer;
}

void turnFonaOff() {
  digitalWrite(PIN_FONA_KEY, HIGH);
}
void turnFonaOn() {
  digitalWrite(PIN_FONA_KEY, LOW);
}

void setErrorLEDOn() {
  digitalWrite(PIN_LED_ERROR, HIGH);
}
void setErrorLEDOff() {
  digitalWrite(PIN_LED_ERROR, LOW);
}

void setProfile1LEDOn() {
  digitalWrite(PIN_LED1, HIGH);
}
void setProfile1LEDOff() {
  digitalWrite(PIN_LED1, LOW);
}

void setProfile2LEDOn() {
  digitalWrite(PIN_LED2, HIGH);
}
void setProfile2LEDOff() {
  digitalWrite(PIN_LED2, LOW);
}

void setProfile3LEDOn() {
  digitalWrite(PIN_LED3, HIGH);
}
void setProfile3LEDOff() {
  digitalWrite(PIN_LED3, LOW);
}

void setProfile4LEDOn() {
  digitalWrite(PIN_LED4, HIGH);
}
void setProfile4LEDOff() {
  digitalWrite(PIN_LED4, LOW);
}
