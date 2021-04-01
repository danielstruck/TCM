#include "inc/HWIO.hpp"
#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>


#define BUTTON_DEBOUNCE_THRESHHOLD (10)

Adafruit_FONA_3G fona = Adafruit_FONA_3G(PIN_FONA_RST);
SoftwareSerial fonaSS = SoftwareSerial(FONA_RX, FONA_TX);
SoftwareSerial *fonaSerial = &fonaSS;
bool fonaOn = false;

void debounceBtn(int pin, int& debounce) {
  if (digitalRead(pin) == LOW && debounce < BUTTON_DEBOUNCE_THRESHHOLD) {
    ++debounce;
  }
  else if (digitalRead(pin) == HIGH && debounce > 0) {
    --debounce;
  }
}

// returns true if the profile button is pressed 
bool profileBtnPressed() {
  static int debounce = 0;
  
  debounceBtn(PIN_BTN_SELECT, debounce);
  
  return debounce == BUTTON_DEBOUNCE_THRESHHOLD;
}

// returns true if the reset button is pressed
bool resetBtnPressed() {
  static int debounce = 0;
  
  debounceBtn(PIN_BTN_RST, debounce);
  
  return debounce == BUTTON_DEBOUNCE_THRESHHOLD;
}

// returns true if the arduino is recieving power from the Fona
bool isPowerOK() {
  return digitalRead(PIN_POWER_INDICATOR) == HIGH;
}

// returns -1 on failure, current battery percentage otherwise
uint16_t getBatteryPercentage() {
  uint16_t vPer;
  if (!fona.getBattPercent(&vPer)) {
    vPer = -1;
  }
//  char str[32]; sprintf(str, "battery = %d", vPer);
//  DEBUG_PRINT(F("battery = ")); DEBUG_PRINTLN(vPer)
  return vPer;
}

void toggleFona() {
  digitalWrite(PIN_FONA_KEY, LOW);
  DEBUG_PRINTLN(F("key -> low"));
  int previousPowerStatus = digitalRead(PIN_FONA_PS);
  delay(6000);
  
  while (!isFonaPowered()) {
    DEBUG_PRINTLN(F("Fona not powered"));
    setLEDs(1, 1, 1, 1, 0); // TODO replace w/ error LED blink code "cannot start fona"
  }
  DEBUG_PRINTLN(F("Fona powered"));
  fonaOn = !fonaOn;
  
  digitalWrite(PIN_FONA_KEY, HIGH);

//  char err[32];
//  sprintf(err, "ps:%d>%d,on:%d", previousPowerStatus, isFonaOn(), fonaOn);
//  DEBUG_PRINTLN(err);
//  DEBUG_PRINT("ps:"); DEBUG_PRINT(previousPowerStatus);
//  DEBUG_PRINT(">"); DEBUG_PRINT(isFonaOn());
//  DEBUG_PRINT(",on:"); DEBUG_PRINT(fonaOn);
//  DEBUG_PRINTLN("");
}

bool isFonaOn() {
  return fonaOn;
}

bool isFonaPowered() {
//  DEBUG_PRINT("PS: "); DEBUG_PRINT(digitalRead(PIN_FONA_PS)); DEBUG_PRINT(" == ");DEBUG_PRINTLN(HIGH);
  return digitalRead(PIN_FONA_PS) == HIGH;
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

// input 1 to turn the corresponding LED on, 0 to turn off, and -1 to continue its current state
void setLEDs(int errorLED, int prof1, int prof2, int prof3, int prof4) {
  if (errorLED == 1)      setErrorLEDOn();
  else if (errorLED == 0) setErrorLEDOff();
  
  if (prof1 == 1)      setProfile1LEDOn();
  else if (prof1 == 0) setProfile1LEDOff();
  
  if (prof2 == 1)      setProfile2LEDOn();
  else if (prof2 == 0) setProfile2LEDOff();
  
  if (prof3 == 1)      setProfile3LEDOn();
  else if (prof3 == 0) setProfile3LEDOff();
  
  if (prof4 == 1)      setProfile4LEDOn();
  else if (prof4 == 0) setProfile4LEDOff();
}

bool detectRisingEdge(bool lastBtnState, bool currentBtnState) {
  return lastBtnState == LOW && currentBtnState == HIGH;
}

bool isProfileBtnRising() {
  static bool lastBtnState = false;

  bool currentBtnState = profileBtnPressed();

  bool risingEdgeDetected = detectRisingEdge(lastBtnState, currentBtnState);
  
  lastBtnState = currentBtnState;

  return risingEdgeDetected;
}

bool isResetBtnRising() {
  static bool lastBtnState = false;

  bool currentBtnState = resetBtnPressed();

  bool risingEdgeDetected = detectRisingEdge(lastBtnState, currentBtnState);
  
  lastBtnState = currentBtnState;

  return risingEdgeDetected;
}
