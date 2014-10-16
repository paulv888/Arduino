/*
 * command.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: pvloon
 */
// Do not remove the include below
#include "command.h"

void streamData();
int streamDataEvent;

void sendValues(uint16_t from,payLoad_Value_Pair command) {
//#define SENSOR_THERMISTOR_0 0			// 6x; 0-5
//#define SENSOR_DHT11_0 6 				// 1 time
//#define SENSOR_SWITCH_0 7				// 6x; 7-13
	unsigned long freezetime = millis();
	for (byte sensorid = 0; sensorid <= 5; sensorid++) {
		command.sensor = SENSOR_THERMISTOR_0 + sensorid;
		command.value = sensorValue[sensorid][0];
		command.age = freezetime - sensorAge[sensorid];
		sendMessageRF(from, command);
	}
	command.sensor = SENSOR_DHT11_0;
	command.value = sensorValue[6][0];
	command.age = freezetime - sensorAge[6];

	sendMessageRF(from, command);
	for (byte switchid = 0; switchid <= 5; switchid++) {
		command.sensor = SENSOR_SWITCH_0 + switchid;
		command.value = switchValue[switchid][0];
		command.age = freezetime - sensorAge[switchid];
		sendMessageRF(from, command);
	}
}

void handle_commandIn(uint16_t from, payLoad_Simple &command) {

	switch (command.command) {
	case COMMAND_STATUSREQUEST: {
		payLoad_Value_Pair acommand;
		if (currentStatus>0)
			acommand.command = COMMAND_STATUSON;
		else
			acommand.command = COMMAND_STATUSOFF;
		command.data = currentStatus;
		sendValues(from, acommand);
	}
		break;
	case COMMAND_DEVICE_STATUS: {
		payLoad_DeviceStatus acommand;
		packCommandRF(acommand);
		sendMessageRF(from, acommand);
	}
		break;
	case COMMAND_CALIBRATE: {
		calibrate(true);
		// result sent in calibrate()
		}
		break;
	case COMMAND_STREAM_DATA_ON:
		streamDataEvent = timer.every(command.data * 1000, streamData);
		break;
	case COMMAND_STREAM_DATA_OFF:
		timer.stop(streamDataEvent);
		break;
	}
}

void packCommandRF(payLoad_DeviceStatus &command) {

	command.command = COMMAND_DEVICE_STATUS;
	command.data = 0;
	command.upTime = (double)millis();
	command.freeMemory = freeMemory();
	command.rfSendErrors = rfSendErrors;
	command.rfTransmitPackets = rfTransmitPackets;
	command.rfReceivePackets = rfReceivePackets;
}

void streamData() {
	static int counter;
	counter++;
	Serial.print("Stream: ");
	Serial.println(counter);
	payLoad_Value_Pair acommand;
	acommand.command = COMMAND_STREAM_DATA;
	acommand.data = 0;
	sendValues(RF_BASE_STATION, acommand);
}
