#include "inc/TemperatureDetection.hpp"
#include "inc/Profile.hpp"
#include "inc/HWIO.hpp"

// Keeps track of the last read temperature from the last call to temp_sense()
int temperatureChamber;

// Returns if the current temperature read is within the lower and upper limits of the current profile, inclusive
bool isTemperatureInsideBoundries() {
  return temperatureChamber >= profile[currentProfile].lower &&
         temperatureChamber <= profile[currentProfile].upper;
}

// Reads the temperature of the thermistor with a resistor value of 10k ohm
void temp_sense()
{
  double Vo;
  double R0 = 10000;
  double T0 = 25 + 273;
  double R1 = 10000;
  double R2, T;
  double beta = 3950;

  const int itterations = 5;
  Vo = 0;
  for (int i = 0; i < itterations; ++i) {
    Vo += analogRead(PIN_THERM_IN);
    delay(1);
  }
  Vo /= itterations;
  R2 = R1 * (1024.0 / Vo - 1.0);
  double denominator = 1.0/T0 + (1.0/beta)*log(R2/R0);
  temperatureChamber = 1.0/denominator - 273.0;

  // print out the raw voltage read by the termistor
//  DEBUG_PRINT(F("vin=")); DEBUG_PRINTLN(Vo);
}
