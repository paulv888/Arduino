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
#define DEBUG_SENSORS true
#define DEBUG_COMMAND true
#define DEBUG_WEB true
#define DEBUG_DEVICE false

///////////////////////////////////
// Device Types
#define DEVICE_COUNT 3

// Arduino Module
#define DEVICE_0_NAME "Arduino"
#define DEVICE_0_IDX 0
#define DEVICE_0_ID 98 							// Shared with vlohome
#define DEVICE_0_TYPE DEV_TYPE_ARDUINO
#define DEVICE_0_UPDATE_TIMER (long)5*60*1000
#define DEVICE_0_CALLBACK aliveMessage
#define DEVICE_0_COMMANDHANDLER arduinoHandler

// DHT11 Temp Hum Sensore
#define DEVICE_1_NAME "DHT11"
#define DEVICE_1_IDX 1
#define DEVICE_1_ID 201 					 	// Shared with vlohome
#define DEVICE_1_TYPE DEV_TYPE_TEMP_HUM
#define DEVICE_1_UPDATE_TIMER (long)15*60*1000
#define DEVICE_1_CALLBACK getTempHumidity
#define DEVICE_1_COMMANDHANDLER dht11Handler
#define DHT11_PIN  6

// The little LED
#define DEVICE_2_NAME "Little Led"
#define DEVICE_2_IDX 2
#define DEVICE_2_ID 202 					 	// Shared with vlohome
#define DEVICE_2_TYPE DEV_TYPE_DIMMER
#define DEVICE_2_UPDATE_TIMER (long)0
#define DEVICE_2_CALLBACK NULL
#define DEVICE_2_COMMANDHANDLER ledHandler
#define LED_PIN 7

//
// Value constants
//
// ADD LABEL TO WEB.CPP!!!
//
#define NO_VALUE -1
#define STATUS 0
#define FREEMEMORY 1
#define UPTIME 2
#define TEMPERATURE 3
#define HUMIDITY 4
#define COMMAND 5
#define INOUT 6
#endif /* CONFIGURATION_H_ */

// Local Errors
#define INFO_NORMAL 100
#define INFO_CALIBRATING 101

// Handler return values
#define HNDLR_WRITE_RESULT 1
#define HNDLR_ERROR -1
#define HNDLR_OK 2




// Sensor setting
/*#define SENSOR_POL_TIME (long)10*1000		// 10 Sec
#define SENSOR_CALIBRATION_INTERVAL (long)12*60*60*1000		// 12 Hours
#define SENSOR_THERMISTOR 1
#define SENSOR_DHT11 2
#define SWITCH_0 0
#define SENSOR_THRESHOLD 30
*/



