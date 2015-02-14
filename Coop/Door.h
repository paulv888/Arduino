/*
 * Door.h
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */

#ifndef DOOR_H_
#define DOOR_H_
#include "configuration.h"
#include "Led.h"
#include "MemoryFree.h"
#include "Web.h"

void doorCallbackT();
void doorCallback(const byte deviceIDidx);
byte doorOnOff(const int commandID);

#endif /* DOOR_H_ */

