/*
 * Door.h
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */

#ifndef DOOR_H_
#define DOOR_H_
#include "configuration.h"
#include "dht11.h"
#include "Led.h"
#include "MemoryFree.h"
#include "InternalTemp.h"
#include "Web.h"

void doorInit(const byte deviceIDidx) ;
void doorCallbackT();
void doorCallback(const byte deviceIDidx);
byte doorHandler(const byte deviceIDidx, const int commandID, const int commandvalue);

#endif /* DOOR_H_ */
