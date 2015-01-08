/*
 * dark.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */
#include "dark.h"

/*
 *
 */
void darkHndlrValues(const byte deviceIDidx, const int commandID) {
	char a[MAX_EXT_DATA];
	int value;
	mdevices[deviceIDidx].setCommand(commandID);
	value = analogRead(mdevices[deviceIDidx].getInput());
	if (value == 1023 || value < 0) {
		if (DEBUG_DEVICE_HAND) Serial.print("S-ERR");
		if (DEBUG_DEVICE_HAND) Serial.print(mdevices[deviceIDidx].getName());
		if (DEBUG_DEVICE_HAND) Serial.print(" ");
		if (DEBUG_DEVICE_HAND) Serial.println(value);
		showStatus(SENSOR_ERROR, deviceIDidx);
	}
	mdevices[deviceIDidx].setValue(value);
	sprintf(a, "{\"C\":\"%i\"}", mdevices[deviceIDidx].commandvalue);
	mdevices[deviceIDidx].setExtData(a);
}

void darkInit(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("darkI");
	if (DEBUG_DEVICE_HAND) Serial.println(deviceIDidx);
	darkHandler(deviceIDidx, COMMAND_GET_VALUE, 0);
//	postMessage(deviceIDidx);
}

void darkCallbackT() {
	darkCallback (DARK_IDX);
}

void darkCallback(const byte deviceIDidx) {

	int prev_status = mdevices[deviceIDidx].status;
//	Serial.println("p");
//	Serial.println(mdevices[deviceIDidx].getExtData());
	darkHandler(deviceIDidx, COMMAND_GET_VALUE, 0);
//	Serial.println("c");
//	Serial.println(mdevices[deviceIDidx].getExtData());
	if (mdevices[deviceIDidx].commandvalue > EEPROMReadInt(deviceIDidx * 6 + 0)) {					// below set point
		mdevices[deviceIDidx].status = STATUS_ON;
		mdevices[deviceIDidx].setCommand(COMMAND_ON);
	} else if (mdevices[deviceIDidx].commandvalue <= (EEPROMReadInt(deviceIDidx * 6 + 0) - EEPROMReadInt(deviceIDidx * 6 + 2))) {	// above set point plus threshold
		mdevices[deviceIDidx].status = STATUS_OFF;
		mdevices[deviceIDidx].setCommand(COMMAND_OFF);
	}
	if (prev_status != mdevices[deviceIDidx].status) {
		postMessage(deviceIDidx);
	}
}

byte darkHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem("darkH ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val: ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	switch (commandID) {
	case COMMAND_VALUE_1:
		EEPROMWriteInt(deviceIDidx * 6 + 0, commandvalue);
		darkHndlrValues(deviceIDidx, commandID);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_2:
	    EEPROMWriteInt(deviceIDidx * 6 + 2, commandvalue);
		darkHndlrValues(deviceIDidx, commandID);
		return HNDLR_OK;
		break;
	case COMMAND_STATUSREQUEST:
	case COMMAND_PING:
		darkHndlrValues(deviceIDidx, commandID);
		postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	case COMMAND_GET_VALUE:
		darkHndlrValues(deviceIDidx, commandID);
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}

