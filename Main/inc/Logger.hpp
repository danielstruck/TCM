#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <stdint.h>


extern int dayMillis;

void logData(int temp);
char* convertMillis(uint32_t milSeconds);

#endif // _LOGGER_HPP_
