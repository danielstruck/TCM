#include "inc/Initializer.hpp"
#include "inc/HWIO.hpp"

#include <SoftwareSerial.h>


uint8_t type;


void setupInitialProfile() {
  
}

void setupFona() {
  turnFonaOn();
  
  while (!Serial);
  fonaSerial->begin(4800);
  if (!fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  type = fona.type();
  Serial.println(F("FONA is OK"));
  
  turnFonaOff();
}

void setupLogger(){
  
}

void setupPins() {
  pinMode(PIN_POWER_INDICATOR, INPUT);
  pinMode(PIN_LED_ERROR, OUTPUT);
//  pinMode(PIN_LED4, OUTPUT);
//  pinMode(PIN_LED3, OUTPUT);
//  pinMode(PIN_LED2, OUTPUT);
//  pinMode(PIN_LED1, OUTPUT);
//  pinMode(PIN_FONA_RST, INPUT);
//  pinMode(PIN_FONA_KEY, OUTPUT);
//  pinMode(PIN_SD_SS, OUTPUT);
//  pinMode(PIN_SD_MOSI, OUTPUT);
//  pinMode(PIN_SD_MISO, INPUT);
//  pinMode(PIN_SD_CLK, OUTPUT);
//  pinMode(PIN_THERM_IN, INPUT);
//  pinMode(PIN_BTN_SELECT, INPUT);
//  pinMode(PIN_BTN_RST, INPUT);
//  pinMode(PIN_FONA_PS, INPUT);
}
