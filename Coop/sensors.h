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
#include <math.h>

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

void calibratetimer();
void calibrate(bool forcecalibrate);
void updateTempSensors();
void setupSwitches();
void updateSwitches();

#endif /* SENSORS_H_ */
