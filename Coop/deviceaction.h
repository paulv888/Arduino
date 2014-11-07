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
//#include "device.h"
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

void arduinoInit(const byte deviceIDidx) ;
void arduinoCallbackT();
void arduinoCallback(const byte deviceIDidx);
byte arduinoHandler(const byte deviceIDidx, const int commandID, const int commandvalue);

void dht11Init(const byte deviceIDidx) ;
void dht11CallbackT();
void dht11Callback(const byte deviceIDidx);
byte dht11Handler(const byte deviceIDidx, const int commandID, const int commandvalue);

void doorInit(const byte deviceIDidx) ;
void doorCallbackT();
void doorCallback(const byte deviceIDidx);
byte doorHandler(const byte deviceIDidx, const int commandID, const int commandvalue);

void doorTimer();

byte findDeviceIndex(const int _deviceID);

extern Device mdevices[];

#endif /* SENSORS_H_ */
