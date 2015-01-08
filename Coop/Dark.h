/*
 * dark.h
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */

#ifndef DARK_H_
#define DARK_H_
#include "configuration.h"
#include "dht.h"
#include "Led.h"
#include "MemoryFree.h"
#include "InternalTemp.h"
#include "Web.h"

void darkInit(const byte deviceIDidx) ;
void darkCallbackT();
void darkCallback(const byte deviceIDidx);
byte darkHandler(const byte deviceIDidx, const int commandID, const int commandvalue);

#endif /* DARK_H_ */
