#include "inc/Initializer.hpp"
#include "inc/HWIO.hpp"

#include <SoftwareSerial.h>

#define FONA_TX 0
#define FONA_RX 1

uint8_t type;

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

void setupFona() {
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      Serial.println(F("FONA 3G (European)")); break;
    default: 
      Serial.println(F("???")); break;
  }
  
  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }
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
