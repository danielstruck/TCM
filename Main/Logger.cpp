#include "Adafruit_FONA.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/HWIO.hpp"
#include <SD.h>
//#include <SPI.h>
//#include <SDCore.h>
//#include "inc/SD.h"

uint32_t lastLog = 0;
uint32_t nextLog = 0;
int dayMillis = 0;

//SD.begin(PIN_SD_SS);
//

//void getTime(){
  
//}

//char *stampAndTemp2entry(uint64_t year, uint64_t month, uint64_t day, uint64_t hour, uint64_t minute, uint64_t second, uint64_t temperature) {
//  char *str = malloc(sizeof(char) * 7);
//  uint64_t total =  temperature |
//                   (second <<  8) |
//                   (minute << 14) |
//                   (hour   << 20) |
//                   (year   << 25) |
//                   (month  << 39) |
//                   (day    << 43);
//  
//  for (int i = 0; i < 6; i++) {
//    str[i] = (char) (total & 0xFF);
//    total >>= 8;
//  }
//
//  str[6] = '\0';
//  return str;
//}
//
//uint64_t *entry2StampAndTemp(char *str) {
//  uint64_t year, month, day, hour, minute, second, temperature;
//  uint64_t total;
//
//  total = str[5];
//  for (int i = 4; i >= 0; i--) {
//    total <<= 8;
//    total |= str[i];
//  }
//
//  temperature = total & 255; // ... & (2^8-1)
//  total >>= 8;
//  second = total & 63; // ... & (2^6-1)
//  total >>= 6;
//  minute = total & 63; // ... & (2^6-1)
//  total >>= 6;
//  hour = total & 31; // ... & (2^5-1)
//  total >>= 5;
//  year = total & 16383; // ... & (2^14-1)
//  total >>= 14;
//  month = total & 15; // ... & (2^4-1)
//  total >>= 4;
//  day = total & 31; // ... & (2^5-1)
//
//  uint64_t *arr = malloc(sizeof(uint64_t) * 7);
//  arr[0] = year;
//  arr[1] = month;
//  arr[2] = day;
//  arr[3] = hour;
//  arr[4] = minute;
//  arr[5] = second;
//  arr[6] = temperature;
//  return arr;
//}

char* convertMillis(uint32_t mils){
  int hourMillis, minuteMillis, secondMillis;
  char str[32];

//  Serial.print("mils: "); Serial.println(mils);
 
  while (mils >= 86400000){
    mils /= 86400000;
    dayMillis++;
  }
  while (mils >= 3600000){
    mils /= 3600000;
    hourMillis++;
  }
  while (mils >= 60000){
    mils /= 60000;
    minuteMillis++;
  }
  while (mils >= 1000){
    mils /= 1000;
    secondMillis++;
  }

  sprintf(str, "%4d %2d %2d %2d", dayMillis, hourMillis, minuteMillis, secondMillis);
//  Serial.println(secondMillis);
//  Serial.println(minuteMillis);
//  Serial.println(hourMillis);
//  Serial.println(dayMillis);
//  Serial.println(str);
  return str;
}

void logData(int temp) {
  uint32_t currentTime = millis();
//  char *str;
  int bat;
  File f1, f2;
//  DEBUG_PRINT("in logger "); DEBUG_PRINT(lastLog); DEBUG_PRINT(" "); DEBUG_PRINT(nextLog); DEBUG_PRINT(" "); DEBUG_PRINTLN(currentTime);

  if ((nextLog < lastLog) && (currentTime >= lastLog)){
    //does nothing
  }
  else if (currentTime >= nextLog)
  {
    DEBUG_PRINTLN("LOG");
//    str = convertMillis(currentTime);
    bat = getBatteryPercentage();
    f1 = SD.open("log1.txt", FILE_WRITE);
//    DEBUG_PRINT(f1); DEBUG_PRINT(" "); DEBUG_PRINT(f1.available()); DEBUG_PRINT(" "); DEBUG_PRINTLN(f1.availableForWrite());
//    f1.print(str);
    f1.print(currentTime); f1.print(" ");
//    Serial.println("@1");
    f1.print(temp); f1.print(" ");
    f1.println(bat);
//    Serial.println("@2");
	  f1.close();

    f2 = SD.open("log2.txt", FILE_WRITE);
//    DEBUG_PRINT(f2); DEBUG_PRINT(" "); DEBUG_PRINT(f2.available()); DEBUG_PRINT(" "); DEBUG_PRINTLN(f2.availableForWrite());
//    f2.print(str);
    f2.print(currentTime); f2.print(" ");
//    Serial.println("@3");
    f2.print(temp); f2.print(" ");
    f2.println(bat);
//    Serial.println("@4");
    f2.close();

    lastLog = currentTime;
    nextLog = lastLog + fifteenSec;
  }
}
