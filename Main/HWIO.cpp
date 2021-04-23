#include "inc/HWIO.hpp"
#include "inc/Errors.h"
#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>


// Debounce constant for number of itterations that the button needs to be held down before being registed as a debounced button press
#define BUTTON_DEBOUNCE_THRESHHOLD (10)

// Global data for the fona
static Adafruit_FONA_3G fona = Adafruit_FONA_3G(PIN_FONA_RST);
static SoftwareSerial fonaSS = SoftwareSerial(FONA_RX, FONA_TX);
static SoftwareSerial *fonaSerial = &fonaSS;
// Keeps track of whether the Fona is on or off
//  DEPRICATED - superceded by reading power state pin (PS) from Fona
static bool fonaOn = false;

// Debounce algorithm for a button that increments {debounce} when the button is pressed
//  and decrements it when the button is not pressed. {debounce} should be compated to
//  BUTTON_DEBOUNCE_THRESHHOLD to determine whether the button has finished with its debounce
void debounceBtn(int pin, uint8_t &debounce) {
  if (digitalRead(pin) == LOW && debounce < BUTTON_DEBOUNCE_THRESHHOLD) {
    ++debounce;
  }
  else if (digitalRead(pin) == HIGH && debounce > 0) {
    --debounce;
  }
}

// returns true if the profile button is pressed 
bool profileBtnPressed() {
  static uint8_t debounce = 0;
  
  debounceBtn(PIN_BTN_SELECT, debounce);
  
  return debounce == BUTTON_DEBOUNCE_THRESHHOLD;
}

// returns true if the reset button is pressed
bool resetBtnPressed() {
  static uint8_t debounce = 0;
  
  debounceBtn(PIN_BTN_RST, debounce);
  
  return debounce == BUTTON_DEBOUNCE_THRESHHOLD;
}

// returns true if the arduino is recieving power from commercial power
bool isPowerOK() {
  return digitalRead(PIN_POWER_INDICATOR) == HIGH;
}

// returns -1 on failure, current battery percentage otherwise
uint16_t getBatteryPercentage() {
  uint16_t vPer;
  
  setFonaOn();
  if (!fona.getBattPercent(&vPer)) {
    vPer = -1;
  }
  setFonaOff();
  
  return vPer;
}

// attempts to turn the Fona on up to 5 times
// NOTE: this may take a long time to complete
void setFonaOn() {
  if (isFonaPowered()) return;
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(PIN_FONA_KEY, LOW);
    DEBUG_PRINTLN(F("key -> low"));
    delay(5000);
    digitalWrite(PIN_FONA_KEY, HIGH);
    delay(2000);

    if (!isFonaPowered()) {
      DEBUG_PRINTLN(F("Fona not turning on"));
      errorFlag |= badSMS;
      blinkLED();// setLEDs(1, 1, 1, 1, 0); // TODO replace w/ error LED blink code "cannot start fona"
    }
    else {
      break;
    }
  }
  
  DEBUG_PRINTLN(F("Fona turned on"));
  fonaOn = true;
}

// attempts to turn the Fona off up to 5 times
// NOTE: this may take a long time to complete
void setFonaOff() {
  if (!isFonaPowered()) return;
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(PIN_FONA_KEY, LOW);
    DEBUG_PRINTLN(F("key -> low"));
    delay(5000);
    digitalWrite(PIN_FONA_KEY, HIGH);
    delay(2000);

    if (isFonaPowered()) {
      DEBUG_PRINTLN(F("Fona not turning off"));
      errorFlag |= badSMS;
      blinkLED();// setLEDs(1, 1, 1, 1, 0); // TODO replace w/ error LED blink code "cannot start fona"
    }
    else {
      break;
    }
  }
  
  DEBUG_PRINTLN(F("Fona turned off"));
  fonaOn = false;
}

// deprecated
bool isFonaOn() {
  return fonaOn;
}

// returns true if the Fona is on, false otherwise
bool isFonaPowered() {
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

// detects a rising edge in a high/low system using the current state and previous state
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
