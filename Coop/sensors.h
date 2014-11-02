/*
 * sensors.h
 *
 *  Created on: Aug 30, 2012
 *      Author: pvloon
 */

#ifndef SENSORS_H_
#define SENSORS_H_
#include "configuration.h"
#include "dht11.h"
#include "Led.h"
#include "device.h"
#include <math.h>
#include "MemoryFree.h"
#include "InternalTemp.h"
#include "Web.h"
#include "Led.h"

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

void aliveMessage();
void getTempHumidity();
void ledOnOff();

void ledOnOff(void *deviceID, void *commandID, void *commandvalue) ;


extern Device devices[];

#endif /* SENSORS_H_ */
