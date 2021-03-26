#include "inc/TemperatureDetection.hpp"

struct constants
{
  int ThermistorPin = 0;
  int Vo;
  int R0 = 10000;
  int T0 = 25 + 273;
  float R1 = 10000;
  float z, R2, T;
  float beta = 3950;
  char UNITS = 'C';
};

int temperatureChamber;
bool temperatureInsideBoundries;

bool isTemperatureInsideBoundries() {
  return temperatureInsideBoundries;
}

void temp_sense(struct constants)
{
  struct constants constants;
  constants.Vo = analogRead(constants.ThermistorPin);
  constants.R2 = constants.R1 * (1024 / (float)constants.Vo - 1.0);
  constants.T = (float)1/constants.T0 + (1/constants.beta)*log((float)constants.R2/constants.R0);
  constants.T = 1/constants.T - 273;
  constants.T = (constants.T*9/5)+32;
  Serial.print("Temperature: "); 
  Serial.print(constants.T);
  Serial.println("F"); 
}
