#include "inc/Initializer.hpp"
#include "inc/HWIO.hpp"
#include "inc/Profile.hpp"
#include "inc/Errors.h"
#include "inc/MessageSender.hpp"
#include <SD.h>


void blinkCurrentProfile() {
  static uint32_t lastBlink = 0;
  static bool state = 0;

  uint32_t ms = millis();
  
  if (lastBlink > lastBlink+500 && ms > lastBlink) {
    /* do nothing */
  }
  else if (ms >= lastBlink+500) {
    state = !state;
    lastBlink += 500;
  }
  
  if (state == 0) {
    setLEDs(0, 0, 0, 0, 0);
  }
  else {
    switch (currentProfile) {
      case 0: 
        setLEDs(0, 1, 0, 0, 0);
      break;
      case 1: 
        setLEDs(0, 0, 1, 0, 0);
      break;
      case 2: 
        setLEDs(0, 0, 0, 1, 0);
      break;
      case 3: 
        setLEDs(0, 0, 0, 0, 1);
      break;
      default:
        setLEDs(0, 1, 1, 1, 1);
    }
  }
}
void setupInitialProfile() {
  DEBUG_PRINTLN(F("> init profile"));
  while (!resetBtnPressed()) {
    if (isProfileBtnRising()) {
      incrementProfile();
    }
    blinkCurrentProfile();
  }
  
  while (resetBtnPressed())
    /* do nothing */;
  
  switch (currentProfile) {
    case 0: 
      setLEDs(0, 1, 0, 0, 0);
    break;
    case 1: 
      setLEDs(0, 0, 1, 0, 0);
    break;
    case 2: 
      setLEDs(0, 0, 0, 1, 0);
    break;
    case 3: 
      setLEDs(0, 0, 0, 0, 1);
    break;
    default:
      setLEDs(0, 1, 1, 1, 1);
  }
}

void setupFona() {
  if (!isFonaPowered()) {
    DEBUG_PRINTLN(F("Turn Fona power on"));
    setFonaOn();
  }
  
  DEBUG_PRINTLN(F("Begin fona serial"));
  fonaSerial->begin(4800);
  
  DEBUG_PRINTLN(F("start up fona communications"));
  bool isFonaNotStarted;
  for (int tries = 3; tries > 0 && (isFonaNotStarted = !fona.begin(*fonaSerial)); tries--)
    DEBUG_PRINTLN(F("fona communications failed"));
  
  if (isFonaNotStarted) {
    setErrorFlag(cannotStartFona);
    blinkLED();
    while (1);
  }
    
//  DEBUG_PRINTLN(F("Enable RTC"));
//  if (!fona.enableRTC(1))
//    DEBUG_PRINTLN(F("Failed to enable RTC"));

//  DEBUG_PRINTLN(F("Enable network time sync"));
//  for (int tries = 5; tries > 0 && !fona.enableNetworkTimeSync(1); tries--)
//    DEBUG_PRINTLN(F("Failed to enable network time sync"));

  DEBUG_PRINTLN(F("fona setup complete"));
}

void setupLogger(){
  SD.begin(PIN_SD_SS);
  SD.open("log1.txt", FILE_WRITE).close();
  SD.open("log2.txt", FILE_WRITE).close();
}

void setupPins() {
  pinMode(PIN_POWER_INDICATOR, INPUT);
  pinMode(PIN_LED_ERROR, OUTPUT);
  pinMode(PIN_LED4, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_FONA_RST, INPUT);
  pinMode(PIN_FONA_KEY, OUTPUT);
  pinMode(PIN_SD_SS, OUTPUT);
  pinMode(PIN_SD_MOSI, OUTPUT);
  pinMode(PIN_SD_MISO, INPUT);
  pinMode(PIN_SD_CLK, OUTPUT);
  pinMode(PIN_THERM_IN, INPUT);
  pinMode(PIN_BTN_SELECT, INPUT_PULLUP); // internal pullup - https://www.arduino.cc/en/Tutorial/Foundations/DigitalPins
  pinMode(PIN_BTN_RST, INPUT_PULLUP);
  pinMode(PIN_FONA_PS, INPUT);

  

//  digitalWrite(PIN_FONA_KEY, LOW);
}
