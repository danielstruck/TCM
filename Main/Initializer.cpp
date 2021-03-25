#include "inc/Initializer.hpp"
#include "inc/HWIO.hpp"
#include "inc/Profile.hpp"

void turnProfileLEDOn() {
  switch (currentProfile) {
    case 0: 
      setProfile1LEDOn();
      break;
    case 1: 
      setProfile2LEDOn();
      break;
    case 2: 
      setProfile3LEDOn();
      break;
    case 3: 
      setProfile4LEDOn();
      break;
  }
}
void setupInitialProfile() {
  setLEDs(1, 0, 0, 0, 0);
  turnProfileLEDOn();
  while (!resetBtnPressed()) {
    if (isProfileBtnRising()) {
      incrementProfile();
      turnProfileLEDOn();
    }
    // TODO blink profile LEDs to signify that rst/start btn must be pressed
  }
}

void setupFona() {
  if (!isFonaPowered()) {
    Serial.println("Turn Fona power on");
    toggleFona();
  }
  
  Serial.println("Begin fona serial");
  fonaSerial->begin(4800);
  
  Serial.println("start up fona communications");
  if (!fona.begin(*fonaSerial)) {
    Serial.println("fona communications failed");
    /*setLEDs(1, 0, 1, 1, 1); */ // TODO replace w/ error LED blink code "cannot start fona"
    while (1);
  }

  Serial.println("Enable time sync");
  if (!fona.enableRTC(1))
    Serial.println("Failed to enable time sync");

  Serial.println("fona setup complete");
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
  pinMode(PIN_BTN_SELECT, INPUT_PULLUP); // internal pullup - https://www.arduino.cc/en/Tutorial/Foundations/DigitalPins
  pinMode(PIN_BTN_RST, INPUT_PULLUP);
  pinMode(PIN_FONA_PS, INPUT);

//  digitalWrite(PIN_FONA_KEY, LOW);
}
