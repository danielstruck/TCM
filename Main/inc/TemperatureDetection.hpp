#ifndef _TEMPERATURE_HPP_
#define _TEMPERATURE_HPP_

// extern struct constants;

extern int temperatureChamber;
extern bool temperatureInsideBoundries;

void temperatureCompare();

bool isTemperatureInsideBoundries();

void temp_sense();
#endif // _TEMPERATURE_HPP_