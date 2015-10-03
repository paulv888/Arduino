/*
 * Door.h
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */

#ifndef DOOR_H_
#define DOOR_H_
#include "Configuration.h"
#include "Led.h"
#include "MemoryFree.h"
#include "Web.h"

void doorTimer(const byte deviceIdx);
byte doorOnOff(const byte deviceIdx, const int commandID);

#endif /* DOOR_H_ */

