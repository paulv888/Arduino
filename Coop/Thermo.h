/*
 * Thermo.h
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */

#ifndef THERMO_H_
#define THERMO_H_
#include "configuration.h"
#include "dht.h"
#include "Led.h"
#include "MemoryFree.h"
#include "InternalTemp.h"
#include "Web.h"

void thermoInit(const byte deviceIDidx) ;
void thermoCallbackT();
void thermoCallback(const byte deviceIDidx);
byte thermoHandler(const byte deviceIDidx, const int commandID, const int commandvalue);

extern void ntcCallback(const byte deviceIDidx);

#endif /* THERMO_H_ */
