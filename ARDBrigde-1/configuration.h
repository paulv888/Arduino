/*
 * configuration.h
 *
 *  Created on: Aug 31, 2012
 *      Author: pvloon
 */
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

// nRF24L01(+) radio attached using Getting Started board
#define RF_CE_PIN 5
#define RF_CSN_PIN 8
#define LED_PIN 13

#define COMMAND_OFF 20
#define COMMAND_ON 17
#define COMMAND_STATUSON 8
#define COMMAND_STATUSOFF 6
#define COMMAND_STATUSREQUEST 85
#define COMMAND_READ_SENSORS 90
#define COMMAND_CONFIGURATION 91


// Address of our node
#define RF_THIS_NODE 0

///////////////////////////////////
// init debugging
#define DEBUG_MAIN false
#define DEBUG_RF false
#define DEBUG_COMMAND false
#define DEBUG_WEB false


///////////////////////////////////
// Device Types
#define ARDBRIDGE 16
#define RANGEARD 17
#define OVENSARD 18

#define DEVICE_COUNT 1					// not counting myself
#define DEVICE_0_ID 98 					// this node
#define DEVICE_1_ID 99					// Range
#define DEVICE_2_ID 100					// Ovens
#define DEVICE_0_TYPE ARDBRIDGE
#define DEVICE_1_TYPE RANGEARD
#define DEVICE_2_TYPE OVENSARD
#define DEVICE_0_NODE_ID 0				// this node
#define DEVICE_1_NODE_ID 1
#define DEVICE_2_NODE_ID 2

#define ALIVE_TIMER 10000
#define FIND_BY_NODE_ADDRESS 1
#define FIND_BY_DEVICE_ID 2


// Structure of our command
struct command_tRF {
	int command;
	unsigned char switches;
	unsigned char minTemp;
	unsigned char sensorTemp[7];
	long maxTemp;
};


#endif /* CONFIGURATION_H_ */
