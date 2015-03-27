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

int millisRollover();
char* malcpy(const char* s);
void EEPROMWriteInt(int p_address, int p_value);
int EEPROMReadInt(int p_address);
byte debounceRead(const int buttonPin);

#endif /* UTILS_H_ */

