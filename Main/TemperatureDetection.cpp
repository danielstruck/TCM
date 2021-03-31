#include "inc/TemperatureDetection.hpp"
#include "inc/Profile.hpp"
#include "inc/HWIO.hpp"


int temperatureChamber;


bool isTemperatureInsideBoundries() {
//  return profile[currentProfile].lower >= temperatureChamber &&
//         profile[currentProfile].upper <= temperatureChamber;
  return temperatureChamber;
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
  temperatureChamber = 1.0/T0 + (1.0/beta)*log(R2/R0);
  temperatureChamber = 1.0/temperatureChamber - 273.0;
//  T = (T*9/5)+32; // convert to F
  Serial.print("Temp: "); Serial.print(temperatureChamber); Serial.print("C  from  Vo="); Serial.println(Vo);
}
