/*
 * Devicehandler.h
 *
 *  Created on: Jan 8, 2015
 *      Author: pvloon
 */

#ifndef DEVICEHANDLER_H_
#define DEVICEHANDLER_H_
#include "configuration.h"
#include "dht.h"
#include "Led.h"
#include "MemoryFree.h"
#include "Web.h"
#include "Coop.h"
#include "Device.h"


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
} // extern "C"
#endif

byte deviceCommandHandler(const byte deviceIDidx, const int commandID, const boolean post, const int commandvalue = 0);
int ReadTemp(const byte deviceIDidx);

void darkCallbackT();
void darkCallback(const byte deviceIDidx);

void arduinoCallbackT();
void arduinoCallback(const byte deviceIDidx);

void dhtCallbackT();
void dhtCallback(const byte deviceIDidx);

class Device;
extern Device mdevices[];
#endif /* DEVICEHANDLER_H_ */
