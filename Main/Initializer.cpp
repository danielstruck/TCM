#include "inc/Initializer.hpp"
#include "inc/HWIO.hpp"



uint8_t type;


void setupInitialProfile() {
  delay(5000);
}

void setupFona() {
  setLEDs(1, 0, 1, 1, 1);
  turnFonaOn();
  setLEDs(1, 1, 0, 1, 1);
  
  fonaSerial->begin(4800);
  setLEDs(1, 0, 0, 1, 1);
  if (!fona.begin(*fonaSerial)) {
    setLEDs(1, 1, 1, 0, 1);
    while (1);
  }
  setLEDs(1, 0, 1, 0, 1);
  
  turnFonaOff();
  setLEDs(1, 1, 0, 1, 1);
}

void setupLogger(){
  
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
  pinMode(PIN_BTN_SELECT, INPUT);
  pinMode(PIN_BTN_RST, INPUT);
  pinMode(PIN_FONA_PS, INPUT);

  digitalWrite(PIN_FONA_KEY, LOW);
}
