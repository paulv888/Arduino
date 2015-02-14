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

// Command direct (seen from server view)
#define COMMAND_IO_RECV 1 		// Received or Incoming
#define COMMAND_IO_SEND 2		// Send or Outgoing

// Commands from Device IN for server
#define COMMAND_PING 151
#define COMMAND_SET_RESULT 285

/*#define COMMAND_RESULT_OK 1
#define COMMAND_RESULT_ERROR 2
#define COMMAND_CALIBRATED 92
*/

// Either
#define STATUS_ON 1
#define STATUS_OFF 0
#define STATUS_UNKNOWN 2
#define STATUS_ERROR -1
#define STATUS_NOT_DEFINED 10		// Used for defining status on commands

// Device Types
/*#define DEV_TYPE_ARDUINO 16
#define DEV_TYPE_TEMP_HUM 11
#define DEV_TYPE_DIMMER_MOTOR 6
#define DEV_TYPE_SWITCH 3
#define DEV_TYPE_THERMOSTAT_HEAT 19
#define DEV_TYPE_DIMMER 1
#define DEV_AUTO_DOOR 30
*
*/
// Errors, passed in data field
/*#define ERROR_CALIBRATE 127					// Fix this is a status and not a command Status Err?
#define ERROR_RF_TIMEOUT 128
#define ERROR_RF_SEND_FAILED 129
*/
#endif /* SHARE_COMMANDS_H_ */


