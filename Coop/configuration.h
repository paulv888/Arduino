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

/*
 *
 *
#define ARDUINO_IDX 0
#define THERMO_IDX 1
#define DOOR_IDX 2
#define DHT_IDX 3
#define RELAY_0_IDX 4
#define RELAY_1_IDX 5
#define RELAY_2_IDX 6
#define AUTO_FAN_IDX 7
#define DARK_IDX 8
#define NTC_0_IDX 9
*
#define SERIAL_IN_PIN			0
#define SERIAL_OUT_PIN			1
#define DIRECTION_RELAY_PIN 	2
#define POWER_RELAY_PIN 		3
#define TOP_SWITCH_PIN 			4
#define BOTTOM_SWITCH_PIN 		5
#define DHT_PIN  				6
#define LED_PIN 				7
#define NTC_0_PIN 				A0
#define PHOTO_RESISTOR_PIN 		A1
#define RELAY_0_PIN			 	A2 -  - Not a device anymore part of Thermo
#define RELAY_HEAT_PIN 			A3
#define RELAY_1_PIN 			A4
#define RELAY_2_PIN 			A5
*/

#define TYPE_DIGITAL_IO  1
#define TYPE_ANALOG_IN   2
#define TYPE_DHT22       3
#define TYPE_THERMO_HEAT 4
#define TYPE_THERMO_COOL 5
#define TYPE_AUTO_DOOR   6
#define TYPE_ARDUINO     7
//#define TYPE_
//#define TYPE_
//#define TYPE_

// Arduino Module
#define ARDUINO_IDX 0
#define LED_PIN 7

// Thermo
#define THERMO_IDX 1
#define NTC_0_PIN A0
#define RELAY_HEAT_PIN A3

// Automatic Door
#define DOOR_IDX 2
#define DIRECTION_RELAY_PIN 2
#define POWER_RELAY_PIN 3
#define TOP_SWITCH_PIN 4
#define BOTTOM_SWITCH_PIN 5

// DHT22 Temp Hum Sensor
#define DHT_IDX 3
#define DHT_PIN  6

// Daylight
#define DARK_IDX 4
#define PHOTO_RESISTOR_PIN A1

// Relays
#define RELAY_0_PIN A2
#define RELAY_1_IDX 5
#define RELAY_1_PIN A4
#define RELAY_2_IDX 6
#define RELAY_2_PIN A5

// Auto Fan
#define AUTO_FAN_IDX 7
#define NTC_0_IDX 8
#define NTC_0_PIN A0

// Local Errors
#define INFO_NORMAL 100
#define INFO_CALIBRATING 101
#define TIMER_ERROR 201
#define SENSOR_ERROR 202
#define DOOR_NOT_MOVING 203

// Handler return values
#define HNDLR_WRITE_RESULT 1
#define ERROR -1
#define HNDLR_OK 2

#define MAX_EXT_DATA 40
#define MAX_NAME_LEN 15
#define MAX_POST_PARAMS 50


// Reserve 3 ints (6 bytes) per DeviceIDX -> Max deviceIDX = 1024/6 = 42
//#define DOOR_DELAY_ADDRESS 			 0
//#define NTC_0_ADDRESS 				 2
//// FREE								 4
//#define THERMO_SET_ADDRESS 			 6
//#define DOOR_MAX_RUNTIME_ADDRESS 	 8
//#define THERMO_THRESHOLD_ADDRESS 	10
//#define DARK_SET_ADDRESS			12
//#define DARK_THRESHOLD_ADDRESS		14



#endif /* ARDBrigde_H_ */

// Sensor setting
/*#define SENSOR_POL_TIME (long)10*1000		// 10 Sec
#define SENSOR_CALIBRATION_INTERVAL (long)12*60*60*1000		// 12 Hours
#define SENSOR_THERMISTOR 1
#define SENSOR_DHT11 2
#define SWITCH_0 0
#define SENSOR_THRESHOLD 30
*/



