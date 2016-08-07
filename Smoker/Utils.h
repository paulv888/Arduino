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

#define PARAMS_PER_DEVICE 16 // 16 integers
#define PARAMS(dIdx, Parameter) (dIdx * PARAMS_PER_DEVICE * 2 + (Parameter - 1) * 2)


int millisRollover();
char* malcpy(const char* s);
void EEPROMWriteInt(int p_address, int p_value);
int EEPROMReadInt(int p_address);
byte debounceRead(const int buttonPin);

#endif /* UTILS_H_ */

