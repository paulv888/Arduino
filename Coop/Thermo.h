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
#include "Web.h"

void thermoCallbackT();
void thermoCallback(const byte deviceIDidx);

#endif /* THERMO_H_ */
