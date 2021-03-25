#include "inc/Initializer.hpp"
#include "inc/HWIO.hpp"
#include "inc/Profile.hpp"
#include "inc/Errors.h"

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
  bool isFonaNotStarted;
  for (int tries = 3; tries > 0 && (isFonaNotStarted = !fona.begin(*fonaSerial)); tries--)
    Serial.println("fona communications failed");
  
  if (isFonaNotStarted) {
    setErrorFlag(cannotStartFona);
    blinkLED();
    while (1);
  }
    
//  Serial.println("Enable RTC");
//  if (!fona.enableRTC(1))
//    Serial.println("Failed to enable RTC");

  Serial.println("Enable network time sync");
  for (int tries = 5; tries > 0 && !fona.enableNetworkTimeSync(1); tries--)
    Serial.println("Failed to enable network time sync");

  Serial.println("fona setup complete");
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
  pinMode(PIN_BTN_SELECT, INPUT_PULLUP); // internal pullup - https://www.arduino.cc/en/Tutorial/Foundations/DigitalPins
  pinMode(PIN_BTN_RST, INPUT_PULLUP);
  pinMode(PIN_FONA_PS, INPUT);

//  digitalWrite(PIN_FONA_KEY, LOW);
}
