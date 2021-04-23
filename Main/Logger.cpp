#include "Adafruit_FONA.h"
#include <stdint.h>
#include <stdlib.h>
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"
#include "inc/Main.hpp"
#include <SD.h>

// keeps track of when to send the next message and allows us to handle overflow in millis()
uint32_t lastLog = 0;
uint32_t nextLog = 0;


void logData(int temp) {
  uint32_t currentTime = millis();
  int bat;
  File f1, f2;
  
  // Handle overflow
  if ((nextLog < lastLog) && (currentTime >= lastLog)){
    //does nothing
  }
  else if (currentTime >= nextLog)
  {
    DEBUG_PRINTLN("LOG");
    bat = getBatteryPercentage();

    // Append data to both files.
    //  Files written with software Raid 1 to reduce risks of corruption
    f1 = SD.open("log1.txt", FILE_WRITE);
    f1.print(currentTime); f1.print(" ");
    f1.print(temp); f1.print(" ");
    f1.println(bat);
	  f1.close();

    f2 = SD.open("log2.txt", FILE_WRITE);
    f2.print(currentTime); f2.print(" ");
    f2.print(temp); f2.print(" ");
    f2.println(bat);
    f2.close();

    lastLog = currentTime;
    nextLog = lastLog + fifteenMinutes;
  }
}
