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
//  Pin definitions
#define DHT11_PIN  6
#define LED_PIN 7

///////////////////////////////////
// init debugging
#define DEBUG_MAIN true
#define DEBUG_SENSORS true
#define DEBUG_COMMAND true
#define DEBUG_WEB true


// Sensor setting
#define SENSOR_POL_TIME (long)10*1000		// 10 Sec
#define SENSOR_CALIBRATION_INTERVAL (long)12*60*60*1000		// 12 Hours
#define SENSOR_THERMISTOR 1
#define SENSOR_DHT11 2
#define SWITCH_0 0
#define SENSOR_THRESHOLD 30

// Device setting
#define ALIVE_TIMER (long)60*60*1000		// 1 Hour

// Local Errors
#define INFO_NORMAL 100
#define INFO_CALIBRATING 101
// Returned errors are in share_command.h

///////////////////////////////////
// Device Types
#define ARDBRIDGE 16
#define RANGEARD 17
#define OVENSARD 18

#define DEVICE_COUNT 2					// not counting myself
#define DEVICE_0_ID 98 					// this node
#define DEVICE_0_TYPE ARDBRIDGE
#define DEVICE_0_NODE_ID 0				// this node
#define MAX_SENSORS_0 1

#define DEVICE_1_ID 99					// Range
#define DEVICE_1_TYPE RANGEARD
#define DEVICE_1_NODE_ID 1
#define MAX_SENSORS_1 13

#define DEVICE_2_ID 100					// Ovens
#define DEVICE_2_TYPE OVENSARD
#define DEVICE_2_NODE_ID 2
#define MAX_SENSORS_2 3


//#define ALIVE_TIMER (long)60*(long)60*(long)1000			// 15 min
#define FIND_BY_NODE_ADDRESS 1
#define FIND_BY_DEVICE_ID 2

#define WEB_POST_TYPE_VALUES 1
#define WEB_POST_TYPE_STATUS 2
#define WEB_POST_TYPE_SIMPLE 3


#define WEB_RESULT_OK 200
#define WEB_RESULT_UNKNOWN_DEVICE 100
#define WEB_RESULT_UNKNOWN_COMMAND 101
#define WEB_RESULT_UNKNOWN_DEV_COM 102
#define WEB_RESULT_DEV_TIMEOUT 103
//#define WEB_RESULT_ 10
//#define WEB_RESULT_ 10


//#define server in Web.cpp
//#define WEB_POST_TO_URL "/cronjobs/str.php?XDEBUG_SESSION_START=ECLIPSE_DBGP&KEY=13478238929791"
#define WEB_POST_TO_URL "/cronjobs/70D455DC-ACB4-4525-8A85-E6009AE93AF4/str.php"

#endif /* CONFIGURATION_H_ */
