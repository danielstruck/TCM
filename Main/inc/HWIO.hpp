#ifndef _HWIO_HPP_
#define _HWIO_HPP_

#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

#define FONA_TX (2)
#define FONA_RX (3)
#define PIN_POWER_INDICATOR ("A4") // used to detect power outages
#define PIN_LED_ERROR  ("A5") // error LED
#define PIN_LED4       (4) // LED for profile 4
#define PIN_LED3       (5) // LED for profile 3
#define PIN_LED2       (6) // LED for profile 2
#define PIN_LED1       (7) // LED for profile 1
#define PIN_FONA_RST   (8) // 
#define PIN_FONA_KEY   (9) // FONA power on/off 
#define PIN_SD_SS      (10) // slave selector for SD breakout
#define PIN_SD_MOSI    (11) // MOSI for SD breakout
#define PIN_SD_MISO    (12) // MISO for SD breakout
#define PIN_SD_CLK     (13) // clock for SD breakout
#define PIN_THERM_IN   ("A0") // thermistor input 
#define PIN_BTN_SELECT ("A1") // profile selector button input 
#define PIN_BTN_RST    ("A2") // reset button input 
#define PIN_FONA_PS    ("A3") // power status pin on Fona

#define BUTTON_DEBOUNCE_THRESHHOLD (10)


extern Adafruit_FONA_3G fona;
extern SoftwareSerial fonaSS;
extern SoftwareSerial *fonaSerial;


// returns true if the profile button is pressed 
bool profileBtnPressed();

// returns true if the reset button is pressed
bool resetBtnPressed();

// returns true if the arduino is recieving power from the Fona
bool isPowerOK();

// returns -1 on failure, current battery percentage otherwise
uint16_t getBatteryPercentage();

void toggleFona();
bool isFonaOn();

bool isFonaPowered();

void setErrorLEDOn();
void setErrorLEDOff();

void setProfile1LEDOn();
void setProfile1LEDOff();
void setProfile2LEDOn();
void setProfile2LEDOff();
void setProfile3LEDOn();
void setProfile3LEDOff();
void setProfile4LEDOn();
void setProfile4LEDOff();

void setLEDs(int errorLED, int prof1, int prof2, int prof3, int prof4);


bool detectRisingEdge(bool lastBtnState, bool currentBtnState); // TODO move this to file Profile?
bool isProfileBtnRising(); // TODO move this to file Profile?
bool isResetBtnRising(); // TODO move this to file Profile?

#endif // _HWIO_HPP_