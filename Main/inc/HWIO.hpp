#ifndef _HWIO_HPP_
#define _HWIO_HPP_

#include "Adafruit_FONA.h"

#define FONA_PS  (A3)
#define FONA_RST (4)

extern Adafruit_FONA_3G fona;

bool profileBtnPressed();

bool resetBtnPressed();

#endif // _HWIO_HPP_