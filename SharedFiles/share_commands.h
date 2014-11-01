/*
 * rf_commands.h
 *
 *  Created on: Sep 7, 2012
 *      Author: pvloon
 */

#ifndef SHARE_COMMANDS_H_
#define SHARE_COMMANDS_H_

// Commands from Web
#define COMMAND_STATUSREQUEST 5
#define COMMAND_DEVICE_STATUS 85
#define COMMAND_CALIBRATE 86

// Commands from Device
#define COMMAND_PING 151

/*#define COMMAND_RESULT_OK 1
#define COMMAND_RESULT_ERROR 2
#define COMMAND_STATUSON 8
#define COMMAND_STATUSOFF 6
#define COMMAND_CALIBRATED 92
*/

// Either
#define STATUS_ON 1
#define STATUS_OFF 0
#define STATUS_UNKNOWN 2
#define STATUS_ERROR -1
#define STATUS_NOT_DEFINED 10		// Used for defining status on commands

// Internal Errors
#define ERROR_READ_SENSOR 130
#define ERROR_CALIBRATE 127

// Value constants
#define NO_VALUE 0
#define FREEMEMORY 1
#define UPTIME 2
#define INTERNALTEMP 3




// Errors, passed in data field
/*#define ERROR_CALIBRATE 127					// Fix this is a status and not a command Status Err?
#define ERROR_RF_TIMEOUT 128
#define ERROR_RF_SEND_FAILED 129
*/
#endif /* SHARE_COMMANDS_H_ */

