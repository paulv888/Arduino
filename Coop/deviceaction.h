/*
 * sensors.h
 *
 *  Created on: Aug 30, 2012
 *      Author: pvloon
 */

#ifndef DEVICEACTIONS_H_
#define DEVICEACTIONS_H_
#include "configuration.h"
#include "dht11.h"
#include "Led.h"
#include "MemoryFree.h"
#include "InternalTemp.h"
#include "Web.h"


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


void relayInit(const byte deviceIDidx) ;
void relayCallbackT();
void relayCallback(const byte deviceIDidx);
byte relayHandler(const byte deviceIDidx, const int commandID, const int commandvalue);

//void ntcInit(const byte deviceIDidx) ;
void ntcInit(const byte deviceIDidx) ;
void ntcCallbackT();
void ntcCallback(const byte deviceIDidx);
byte ntcHandler(const byte deviceIDidx, const int commandID, const int commandvalue);

byte findDeviceIndex(const int _deviceID);


extern Device mdevices[];

#endif /* DEVICEACTIONS_H_ */
