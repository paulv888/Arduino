/*
 * configuration.h
 *
 *  Created on: Aug 31, 2012
 *      Author: pvloon
 */
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#include "Arduino.h"

///////////////////////////////////
// init debugging
#define DEBUG_MAIN true
#define DEBUG_DEVICE_HAND true
#define DEBUG_WEB true
#define DEBUG_DEVICE true
#define DEBUG_MEMORY true

///////////////////////////////////
// Device Types
#define DEVICE_COUNT 9

// Arduino Module
#define LED_PIN 7

// DHT11 Temp Hum Sensore
#define DHT11_PIN  6

// Automatic Door
#define DIRECTION_RELAY 2
#define POWER_RELAY 3
#define TOP_SWITCH 4
#define BOTTOM_SWITCH 5

// Local Errors
#define INFO_NORMAL 100
#define INFO_CALIBRATING 101
#define TIMER_ERROR 102
#define SENSOR_ERROR 103

// Handler return values
#define HNDLR_WRITE_RESULT 1
#define ERROR 255
#define HNDLR_OK 2
#define NO_VALUE -12399

#define MAX_EXT_DATA 40
#define MAX_NAME_LEN 10

#define RELAY_0_IDX 3
#define RELAY_1_IDX 4
#define RELAY_2_IDX 5

#define NTC_0_IDX 7
#define NTC_1_IDX 8


#endif /* ARDBrigde_H_ */

// Sensor setting
/*#define SENSOR_POL_TIME (long)10*1000		// 10 Sec
#define SENSOR_CALIBRATION_INTERVAL (long)12*60*60*1000		// 12 Hours
#define SENSOR_THERMISTOR 1
#define SENSOR_DHT11 2
#define SWITCH_0 0
#define SENSOR_THRESHOLD 30
*/



