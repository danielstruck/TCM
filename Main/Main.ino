#include "inc/Main.hpp"
#include "inc/Boundry.hpp"
#include "inc/HWIO.hpp"
#include "inc/Initializer.hpp"
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/Profile.hpp"
#include "inc/TemperatureDetection.hpp"

void setup() {
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

void loop() {
  // check termal range
  temperatureCompare();
  
  if (!isTemperatureInsideBoundries()) {
    // TODO message sender flag
  }

  // TODO daniel log temperature
  logData();

  // TODO daniel handle power outage
  
  // read button to change current profile
  if (profileBtnPressed()) {
    incrementProfile();
  }

  if (!isPowerOK()) {
    chooseMessage(events.badPower);
    sendText();
  }
}
