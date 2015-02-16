/*
 * Devicehandler.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: pvloon
 */
#include "Devicehandler.h"

void reportTimer(const byte reportType) {
//#define REPORT_HOURLY 1
//#define REPORT_DAILY 2
	if (DEBUG_DEVICE_HAND) Serial.print("Rpt ");
	for (byte deviceIdx = 0; deviceIdx < DEVICE_COUNT; deviceIdx++) {
		if (mdevices[deviceIdx].getReportType() == reportType) {
			if (DEBUG_DEVICE_HAND) Serial.print(deviceIdx);
			if (DEBUG_DEVICE_HAND) Serial.print("/");
			deviceCommandHandler(deviceIdx, COMMAND_PING, true);
		}
	}
	if (DEBUG_DEVICE_HAND) Serial.println();
}

void checkTimer(const byte reportType) {
	if (DEBUG_DEVICE_HAND) Serial.print("Chk ");
	for (byte deviceIdx = 0; deviceIdx < DEVICE_COUNT; deviceIdx++) {
		if (mdevices[deviceIdx].getCheckType() == reportType) {
			if (DEBUG_DEVICE_HAND) Serial.print(deviceIdx);
			if (DEBUG_DEVICE_HAND) Serial.print("/");
			mdevices[deviceIdx].readInput();
		}
	}
	if (DEBUG_DEVICE_HAND) Serial.println();
}

byte deviceCommandHandler(const byte deviceIdx, const int commandID, const boolean post, const int commandvalue) {
	if (DEBUG_MEMORY || DEBUG_DEVICE_HAND) printMem("devH ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIdx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val: ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	mdevices[deviceIdx].setCommand(commandID);
	switch (commandID) {
	case COMMAND_ON:
		mdevices[deviceIdx].setOnOff(commandID);
		if (post) postMessage(deviceIdx);
		return HNDLR_OK;
		break;
	case COMMAND_OFF:
		mdevices[deviceIdx].setOnOff(commandID);
		if (post) postMessage(deviceIdx);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_1:
		EEPROMWriteInt(deviceIdx * 6 + 0, commandvalue);
		mdevices[deviceIdx].readInput();
		if (post) postMessage(deviceIdx);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_2:
	    EEPROMWriteInt(deviceIdx * 6 + 2, commandvalue);
		mdevices[deviceIdx].readInput();
		if (post) postMessage(deviceIdx);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_3:
	    EEPROMWriteInt(deviceIdx * 6 + 4, commandvalue);
		mdevices[deviceIdx].readInput();
		if (post) postMessage(deviceIdx);
		return HNDLR_OK;
		break;
	case COMMAND_STATUSREQUEST:
	case COMMAND_SET_RESULT:
	case COMMAND_PING:
		mdevices[deviceIdx].readInput();
		if (post) postMessage(deviceIdx);
		return HNDLR_WRITE_RESULT;
		break;
	case COMMAND_GET_VALUE:
		mdevices[deviceIdx].readInput();
		if (post) postMessage(deviceIdx);
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}

