/*
 * configuration.h
 *
 *  Created on: Aug 31, 2012
 *      Author: pvloon
 */
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#include "Arduino.h"
#include <SoftwareSerial.h>

#define DBG_BAUD_RATE 9600 			// software Serial can only go up to 9600bps

///////////////////////////////////
// init debugging
//#define DEBUG_MAIN true
//#define DEBUG_DEVICE_HAND true
//#define DEBUG_WEB true
//#define DEBUG_DEVICE true
//#define DEBUG_MEMORY true
//#define DEBUG_WIFI true

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
#ifndef DEBUG_WIFI
#define DEBUG_WIFI false
#endif

//#define WDT_ENABLED true
#ifndef WDT_ENABLED
#define WDT_ENABLED false
#endif
 

///////////////////////////////////
// Device Types
#define DEVICE_COUNT 2

#define TYPE_DIGITAL_IO  1
#define TYPE_ANALOG_IN   2
#define TYPE_DHT22       3
#define TYPE_THERMO_HEAT 4
#define TYPE_THERMO_COOL 5
#define TYPE_AUTO_DOOR   6
#define TYPE_ARDUINO     7
#define TYPE_NTC	     8
#define TYPE_TEMP_18B20  9
//#define TYPE_
//#define TYPE_
//#define TYPE_

// Arduino Module
#define ARDUINO_IDX 0
#define LED_PIN 13

// Temp 18B20
#define ONE_WIRE_BUS 4
#define TEMP_1_IDX 1

// Automatic Door
#ifdef COOP
	#define DOOR_IDX 1
	#define DIRECTION_RELAY_PIN 2
	#define POWER_RELAY_PIN 3
	#define TOP_SWITCH_PIN 4
	#define BOTTOM_SWITCH_PIN 5

	// Water Heater
	#define THERMO_IDX 2
	#define NTC_0_PIN A0
	#define RELAY_HEAT_PIN A3

	// Auto Fan
	#define AUTO_FAN_IDX 3
	#define RELAY_FAN_PIN 0

	// Water Level
	#define WATER_LEVEL_IDX 4
	#define PRESSURE_MPX4250GP    A2

	// Daylight Dark
	#define DARK_IDX 5
	#define PHOTO_RESISTOR_PIN A1

	// Relays
	// Coop Light
	#define RELAY_1_IDX 6
	#define RELAY_1_PIN A4
	// Red Light
	#define RELAY_2_IDX 7
	#define RELAY_2_PIN A5

	// DHT22 Temp Hum Sensor
	#define DHT_IDX 8
	#define DHT_PIN 6

	// NTC Temp Sensor
	#define NTC_0_IDX 9
	#define NTC_0_PIN A0
#endif

// Local Errors
#define INFO_NORMAL 100
#define INFO_CALIBRATING 101
#define TIMER_ERROR 201
#define SENSOR_ERROR 202
#define DOOR_NOT_MOVING 203
#define DOOR_MAX_RUNTIME 204
#define NET_ERROR 205

// Handler return values
#define HNDLR_WRITE_RESULT 1
#define ERROR -1
#define HNDLR_OK 2

#define MAX_STRING_LEN 50

#define PARAMS_PER_DEVICE 5 // 5 integers
#define PARAMS(dIdx, Parameter) (dIdx * PARAMS_PER_DEVICE * 2 + (Parameter - 1) * 2)

#define FFFF 0XFFFF

#endif /* ARDBrigde_H_ */