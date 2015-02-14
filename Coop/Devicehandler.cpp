/*
 * Devicehandler.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: pvloon
 */
#include "Devicehandler.h"

int ReadTemp(const byte deviceIDidx) {
	int value = 0;
	if (deviceIDidx == NTC_0_IDX) {
		value = 1024 - analogRead(NTC_0_PIN);
	}
	if (deviceIDidx == DHT_IDX) {
		deviceCommandHandler(deviceIDidx, COMMAND_GET_VALUE, false);
		if (mdevices[deviceIDidx].status == STATUS_ERROR) {
			return ERROR;
		}
		value = mdevices[deviceIDidx].commandvalue;
	}
	return value;
}

void arduinoCallbackT() {
	arduinoCallback (ARDUINO_IDX);
}

void arduinoCallback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("ArdC");
	deviceCommandHandler(deviceIDidx, COMMAND_PING, true);
}

void dhtCallbackT() {
	dhtCallback(DHT_IDX);
}

void dhtCallback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("dhtC");
	deviceCommandHandler(deviceIDidx, COMMAND_PING, true);
}


void darkCallbackT() {
	analogCallback(DARK_IDX);
}

void waterlevelCallbackT() {
	analogCallback (WATER_LEVEL_IDX);
}

void analogCallback(const byte deviceIDidx) {
	int prev_status = mdevices[deviceIDidx].status;
	mdevices[deviceIDidx].readInput();
	if (prev_status != mdevices[deviceIDidx].status) {
		deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, true);
	}
}

byte deviceCommandHandler(const byte deviceIDidx, const int commandID, const boolean post, const int commandvalue) {
	if (DEBUG_MEMORY) printMem("devH ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val: ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	mdevices[deviceIDidx].setCommand(commandID);
	switch (commandID) {
	case COMMAND_ON:
		mdevices[deviceIDidx].setOnOff(commandID);
		if (post) postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_OFF:
		mdevices[deviceIDidx].setOnOff(commandID);
		if (post) postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_1:
		EEPROMWriteInt(deviceIDidx * 6 + 0, commandvalue);
		mdevices[deviceIDidx].readInput();
		if (post) postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_2:
	    EEPROMWriteInt(deviceIDidx * 6 + 2, commandvalue);
		mdevices[deviceIDidx].readInput();
		if (post) postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_3:
	    EEPROMWriteInt(deviceIDidx * 6 + 4, commandvalue);
		mdevices[deviceIDidx].readInput();
		if (post) postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_STATUSREQUEST:
	case COMMAND_SET_RESULT:
	case COMMAND_PING:
		mdevices[deviceIDidx].readInput();
		if (post) postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	case COMMAND_GET_VALUE:
		mdevices[deviceIDidx].readInput();
		if (post) postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}

