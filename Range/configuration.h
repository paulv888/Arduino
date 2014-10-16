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
#define RF_CE_PIN 9
#define RF_CSN_PIN 10


///////////////////////////////////
// init debugging
#define DEBUG_MAIN true
#define DEBUG_RF true
#define DEBUG_SENSORS true
#define DEBUG_COMMAND true


// Sensor setting
#define SENSOR_POL_TIME (long)10*1000		// 10 Sec
#define SENSOR_CALIBRATION_INTERVAL (long)12*60*60*1000		// 12 Hours
#define SENSOR_THERMISTOR 1
#define SENSOR_DHT11 2
#define SWITCH_0 0
#define SENSOR_THRESHOLD 30

// Device setting
// Address of our node
#define RF_THIS_NODE (uint16_t)1
#define RF_BASE_STATION (uint16_t)0
#define ALIVE_TIMER (long)60*60*1000		// 1 Hour

// Local Errors
#define INFO_NORMAL 100
#define INFO_CALIBRATING 101
#define WARNING_RF_SEND 200
// Returned errors are in share_command.h


#endif /* CONFIGURATION_H_ */
