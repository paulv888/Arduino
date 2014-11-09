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
//#define DEBUG_MAIN true
//#define DEBUG_DEVICE_HAND true
//#define DEBUG_WEB true
//#define DEBUG_DEVICE true
//#define DEBUG_MEMORY true
//#define DEBUG_TIMER true

#ifndef DEBUG_MAIN
#define DEBUG_MAIN false
#endif
#ifndef DEBUG_DEVICE_HAND
#define DEBUG_DEVICE_HAND false
#endif
#ifndef DEBUG_WEB
#define DEBUG_WEB false
#endif
#ifndef DEBUG_DEVICE
#define DEBUG_DEVICE false
#endif
#ifndef DEBUG_MEMORY
#define DEBUG_MEMORY false
#endif
#ifndef DEBUG_TIMER
#define DEBUG_TIMER false
#endif


///////////////////////////////////
// Device Types
#define DEVICE_COUNT 9

// Arduino Module
#define ARDUINO_IDX 0
#define LED_PIN 7

// DHT11 Temp Hum Sensore
#define DHT11_IDX 1
#define DHT11_PIN  6

// Automatic Door
#define DOOR_IDX 2
#define DIRECTION_RELAY 2
#define POWER_RELAY 3
#define TOP_SWITCH 4
#define BOTTOM_SWITCH 5
#define DOOR_DELAY_ADDRESS 0

// Relays
#define RELAY_0_IDX 3
#define RELAY_1_IDX 4
#define RELAY_2_IDX 5

// Heater
#define HEATER_IDX 6

// NTCs
#define NTC_0_IDX 7
#define NTC_1_IDX 8

// Local Errors
#define INFO_NORMAL 100
#define INFO_CALIBRATING 101
#define TIMER_ERROR 201
#define SENSOR_ERROR 202
#define DOOR_NOT_MOVING 203

// Handler return values
#define HNDLR_WRITE_RESULT 1
#define ERROR 255
#define HNDLR_OK 2
#define NO_VALUE -12399

#define MAX_EXT_DATA 40
#define MAX_NAME_LEN 10
#define MAX_POST_PARAMS 50


#endif /* ARDBrigde_H_ */

// Sensor setting
/*#define SENSOR_POL_TIME (long)10*1000		// 10 Sec
#define SENSOR_CALIBRATION_INTERVAL (long)12*60*60*1000		// 12 Hours
#define SENSOR_THERMISTOR 1
#define SENSOR_DHT11 2
#define SWITCH_0 0
#define SENSOR_THRESHOLD 30
*/



