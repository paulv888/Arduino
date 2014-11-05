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
#define DEBUG_DEVICE false
#define DEBUG_MEMORY true

///////////////////////////////////
// Device Types
#define DEVICE_COUNT 4

// Arduino Module
#define DEVICE_0_NAME "Arduino"
#define DEVICE_0_IDX (uint8_t)0
#define DEVICE_0_ID 98 							// Shared with vlohome
#define DEVICE_0_TYPE DEV_TYPE_ARDUINO
#define DEVICE_0_UPDATE_TIMER (long)5*60*1000
#define DEVICE_0_CALLBACK arduinoCallback
#define DEVICE_0_COMMANDHANDLER arduinoHandler
#define DEVICE_0_INIT NULL

// DHT11 Temp Hum Sensore
#define DEVICE_1_NAME "DHT11"
#define DEVICE_1_IDX (uint8_t)1
#define DEVICE_1_ID 201 					 	// Shared with vlohome
#define DEVICE_1_TYPE DEV_TYPE_TEMP_HUM
#define DEVICE_1_UPDATE_TIMER (long)15*60*1000
#define DEVICE_1_CALLBACK dht11Callback
#define DEVICE_1_COMMANDHANDLER dht11Handler
#define DEVICE_1_INIT NULL
#define DHT11_PIN  6

// The little LED
#define DEVICE_2_NAME "Little Led"
#define DEVICE_2_IDX (uint8_t)2
#define DEVICE_2_ID 202 					 	// Shared with vlohome
#define DEVICE_2_TYPE DEV_TYPE_DIMMER
#define DEVICE_2_UPDATE_TIMER (long)0
#define DEVICE_2_CALLBACK NULL
#define DEVICE_2_COMMANDHANDLER ledHandler
#define DEVICE_2_INIT ledInit
#define LED_PIN 7

// Automatic Door
#define DEVICE_3_NAME "Coop Door"
#define DEVICE_3_IDX (uint8_t)3
#define DEVICE_3_ID 203 					 	// Shared with vlohome
#define DEVICE_3_TYPE DEV_AUTO_DOOR
#define DEVICE_3_UPDATE_TIMER (long)0
#define DEVICE_3_CALLBACK NULL
#define DEVICE_3_COMMANDHANDLER doorHandler
#define DEVICE_3_INIT doorInit
#define POWER_RELAY 2
#define DIRECTION_RELAY 3
#define TOP_SWITCH 4
#define BOTTOM_SWITCH 5

//
// Value constants
//
// ADD LABEL TO WEB.CPP!!!
//

#define STATUS 0
#define FREEMEMORY 1
#define UPTIME 2
#define TEMPERATURE 3
#define HUMIDITY 4
#define COMMAND 5
#define INOUT 6
#define EXTDATA 7
#endif /* CONFIGURATION_H_ */

// Local Errors
#define INFO_NORMAL 100
#define INFO_CALIBRATING 101

// Handler return values
#define HNDLR_WRITE_RESULT 1
#define ERROR 255
#define HNDLR_OK 2




// Sensor setting
/*#define SENSOR_POL_TIME (long)10*1000		// 10 Sec
#define SENSOR_CALIBRATION_INTERVAL (long)12*60*60*1000		// 12 Hours
#define SENSOR_THERMISTOR 1
#define SENSOR_DHT11 2
#define SWITCH_0 0
#define SENSOR_THRESHOLD 30
*/



