/*
 * rf_commands.h
 *
 *  Created on: Sep 7, 2012
 *      Author: pvloon
 */
// These values are shared with vlohome site
#ifndef SHARE_COMMANDS_H_
#define SHARE_COMMANDS_H_

// Commands from Web OUT for server
#define COMMAND_STATUSREQUEST 5
#define COMMAND_DEVICE_STATUS 85
#define COMMAND_CALIBRATE 86
#define COMMAND_ON 17
#define COMMAND_OFF 20
#define COMMAND_UP 22
#define COMMAND_STOP 58
#define COMMAND_DOWN 26
#define COMMAND_GET_VALUE 136
#define COMMAND_VALUE_1 290
#define COMMAND_VALUE_2 291
#define COMMAND_VALUE_3 296
#define COMMAND_VALUE_4 299
#define COMMAND_VALUE_5 300

// Command direct (seen from server view)
#define COMMAND_IO_RECV 1 		// Received or Incoming
#define COMMAND_IO_SEND 2		// Send or Outgoing

// Commands from Device IN for server
#define COMMAND_PING 151
#define COMMAND_SET_RESULT 285

// Either
#define STATUS_ON 1
#define STATUS_OFF 0
#define STATUS_UNKNOWN 2
#define STATUS_ERROR -1
#define STATUS_NOT_DEFINED 10		// Used for defining status on commands

// Report types
#define REPORT_NONE 0
#define REPORT_HOURLY 1
#define REPORT_DAILY 2

// Check types
#define CHECK_NONE 0
#define CHECK_15_SEC 1
#define CHECK_1_MIN 2
#define CHECK_OWN_TIMER 3

// Device Types
/*
#define DEV_TYPE_ARDUINO 16
#define DEV_TYPE_TEMP_HUM 11
#define DEV_TYPE_DIMMER_MOTOR 6
#define DEV_TYPE_SWITCH 3
#define DEV_TYPE_THERMOSTAT_HEAT 19
#define DEV_TYPE_DIMMER 1
#define DEV_AUTO_DOOR 30
*/
#endif /* SHARE_COMMANDS_H_ */


