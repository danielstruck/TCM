#ifndef _HWIO_HPP_
#define _HWIO_HPP_

#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

#define FONA_TX (0)
#define FONA_RX (1)
#define PIN_POWER_INDICATOR (2) // used to detect power outages
#define PIN_LED_ERROR  (3) // error LED
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
#define PIN_THERM_IN   (14) // thermistor input 
#define PIN_BTN_SELECT (15) // profile selector button input 
#define PIN_BTN_RST    (16) // reset button input 
#define PIN_FONA_PS    (17) // power status pin on Fona

#define BUTTON_DEBOUNCE_THRESHHOLD (100)


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

void turnFonaOff();
void turnFonaOn();

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

#endif // _HWIO_HPP_