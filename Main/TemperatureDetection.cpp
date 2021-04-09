#include "inc/TemperatureDetection.hpp"
#include "inc/Profile.hpp"
#include "inc/HWIO.hpp"


int temperatureChamber;

bool isTemperatureInsideBoundries() {
  return temperatureChamber >= profile[currentProfile].lower &&
         temperatureChamber <= profile[currentProfile].upper;
}

void temp_sense()
{
  double Vo;
  double R0 = 10000;
  double T0 = 25 + 273;
  double R1 = 10000;
  double R2, T;
  double beta = 3950;
  
  Vo = analogRead(PIN_THERM_IN);
  R2 = R1 * (1024.0 / Vo - 1.0);
  double denominator = 1.0/T0 + (1.0/beta)*log(R2/R0);
  temperatureChamber = 1.0/denominator - 273.0;
}
