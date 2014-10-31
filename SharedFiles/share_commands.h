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
#define COMMAND_STREAM_DATA_ON 89
#define COMMAND_STREAM_DATA_OFF 90
// Commands from Device
#define COMMAND_RESULT_OK 1
#define COMMAND_RESULT_ERROR 2
#define COMMAND_STATUSON 8
#define COMMAND_STATUSOFF 6
#define COMMAND_STREAM_DATA 91
#define COMMAND_CALIBRATED 92

// Either
#define COMMAND_OFF 20
#define COMMAND_ON 17

// Errors, passed in data field
#define ERROR_CALIBRATE 127					// Fix this is a status and not a command Status Err?
#define ERROR_RF_TIMEOUT 128
#define ERROR_RF_SEND_FAILED 129
#define ERROR_READ_SENSOR 130

// Sensor #'s
#define SENSOR_THERMISTOR_0 0			// 6x; 0-5
#define SENSOR_DHT11_0 6 				// 1 time
#define SENSOR_SWITCH_0 7				// 6x; 7-13
// Structure of our command
#define PAYLOAD_TYPE_SIMPLE 1
struct payLoad_Simple {
	byte command;
	byte data;
	/* shared between payloads */
};

// Structure of our command
#define PAYLOAD_TYPE_VALUES 2
struct payLoad_Value_Pair {
	byte command;
	byte data;
	/* shared between payloads */
	byte sensor;
	int value;
	unsigned int age;
};

// Structure of our command
#define PAYLOAD_TYPE_DEVICE_STATUS 3
struct payLoad_DeviceStatus {
	byte command;
	byte data;
	/* shared between payloads */
	double upTime;
	int freeMemory;
};

#endif /* RF_COMMANDS_H_ */
