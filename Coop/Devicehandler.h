/*
 * Devicehandler.h
 *
 *  Created on: Jan 8, 2015
 *      Author: pvloon
 */

#ifndef DEVICEHANDLER_H_
#define DEVICEHANDLER_H_
#include "Configuration.h"
#include <Event.h>
#include <Timer.h>
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

byte deviceCommandHandler(const byte deviceIdx, const int commandID, const boolean post, const int commandvalue = 0);
void checkTimer(const byte deviceIdx);
void reportTimer(const byte deviceIdx);

class Device;
extern Device mdevices[];
#endif /* DEVICEHANDLER_H_ */

