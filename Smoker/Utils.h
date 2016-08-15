/*
 * Utils.h
 *
 *  Created on: Sep 3, 2012
 *      Author: pvloon
 */

#ifndef UTILS_H_
#define UTILS_H_
#include "Arduino.h"
#include <EEPROM.h>

#define PARAMS_PER_DEVICE 8 // 16 integers
#define PARAMS(dIdx, Parameter) (dIdx * PARAMS_PER_DEVICE * 2 + (Parameter - 1) * 2)

#define DEBUGPRINT_LF(STR) \
  {  \
    if (DEBUG) Serial.println(STR); \
  }
#define DEBUGPRINT(STR) \
  { \
    if (DEBUG) Serial.print(STR); \
  }


int millisRollover();
char* malcpy(const char* s);
void EEPROMWriteInt(int p_address, int p_value);
int EEPROMReadInt(int p_address);
int toFahrenheit(int temp, byte celcius);
int toCelcius(int temp, byte celcius);
	
#endif /* UTILS_H_ */

