#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <stdint.h>

#define fifteenSec (15000)

extern int dayMillis;
//char errorStatus[32];
// extern int temperatureChamber;

void logData(int temp);
char* convertMillis(uint32_t milSeconds);

#endif // _LOGGER_HPP_
