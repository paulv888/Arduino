/*
 * thermo.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */
#include "thermo.h"

void tHndlrValues(const byte deviceIDidx, const int commandID) {
	char a[MAX_EXT_DATA];
	mdevices[deviceIDidx].setCommand(commandID);
	mdevices[deviceIDidx].setValue(ReadTemp(mdevices[deviceIDidx].getInput()));
	sprintf(a, "{\"C\":\"%i\",\"R\":\"%i\"}", mdevices[deviceIDidx].commandvalue, digitalRead(mdevices[deviceIDidx].getPin()));
	mdevices[deviceIDidx].setExtData(a);
}

void thermoInit(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("thermoI");
	if (DEBUG_DEVICE_HAND) Serial.println(deviceIDidx);
	pinMode(mdevices[deviceIDidx].getPin(), OUTPUT);
	thermoHandler(deviceIDidx, COMMAND_ON, 0);
	postMessage(deviceIDidx);
}

void thermoCallbackT() {
	thermoCallback (THERMO_IDX);
	thermoCallback (AUTO_FAN_IDX);
}

void thermoCallback(const byte deviceIDidx) {

	int isRunning;
	isRunning = digitalRead(mdevices[deviceIDidx].getPin());

	if (mdevices[deviceIDidx].status) {
		int value;
		value = ReadTemp(mdevices[deviceIDidx].getInput());
		if (mdevices[deviceIDidx].getType() == THERMO_HEAT) {
			if (value < EEPROMReadInt(deviceIDidx * 6 + 0)) {					// below set point
				if (!isRunning) {				// switch on
					digitalWrite(mdevices[deviceIDidx].getPin(), HIGH);
					thermoHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
				}
			} else {
				if (value >= (EEPROMReadInt(deviceIDidx * 6 + 0) - EEPROMReadInt(deviceIDidx * 6 + 2))) {	// above set point plus threshold
					if (isRunning) {				// switch off
						digitalWrite(mdevices[deviceIDidx].getPin(), LOW);
						thermoHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
					}

				}

			}
		} else {
			if (value > EEPROMReadInt(deviceIDidx * 6 + 0)) {					// below set point
				if (!isRunning) {				// switch on
					digitalWrite(mdevices[deviceIDidx].getPin(), HIGH);
					thermoHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
				}
			} else {
				if (value <= (EEPROMReadInt(deviceIDidx * 6 + 0) - EEPROMReadInt(deviceIDidx * 6 + 2))) {	// above set point plus threshold
					if (isRunning) {				// switch off
						digitalWrite(mdevices[deviceIDidx].getPin(), LOW);
						thermoHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
					}

				}

			}
		}
	} else {
		if (isRunning) {				// switch off
			digitalWrite(mdevices[deviceIDidx].getPin(), LOW);
			thermoHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
		}
	}
// nothing to do
}

byte thermoHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem("thermoH ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val: ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	switch (commandID) {
	case COMMAND_ON:													// Open
		mdevices[deviceIDidx].setStatus(STATUS_ON);
		tHndlrValues(deviceIDidx, commandID);
		return HNDLR_OK;
		break;
	case COMMAND_OFF:													// Closed
		mdevices[deviceIDidx].setStatus(STATUS_OFF);
		tHndlrValues(deviceIDidx, commandID);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_1:
		EEPROMWriteInt(deviceIDidx * 6 + 0, commandvalue);
		tHndlrValues(deviceIDidx, commandID);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_2:
	    EEPROMWriteInt(deviceIDidx * 6 + 2, commandvalue);
		tHndlrValues(deviceIDidx, commandID);
		return HNDLR_OK;
		break;
	case COMMAND_STATUSREQUEST:
	case COMMAND_SET_RESULT:
	case COMMAND_PING:
		tHndlrValues(deviceIDidx, commandID);
		postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	case COMMAND_GET_VALUE:
		tHndlrValues(deviceIDidx, commandID);
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}

int ReadTemp(const byte deviceIDidx) {
	int value = 0;
	if (deviceIDidx == NTC_0_IDX) {
		value = analogRead(NTC_0_PIN);
		if (value == 1023 || value < 0) {
			if (DEBUG_DEVICE_HAND) Serial.print("S-ERR");
			if (DEBUG_DEVICE_HAND) Serial.print(mdevices[deviceIDidx].getName());
			if (DEBUG_DEVICE_HAND) Serial.print(" ");
			if (DEBUG_DEVICE_HAND) Serial.println(value);
			showStatus(SENSOR_ERROR, deviceIDidx);
			return ERROR;
		}
		value = 1024 - (value + EEPROMReadInt(deviceIDidx * 6 + 0));
	}
	if (deviceIDidx == DHT_IDX) {
		dhtHandler(deviceIDidx, COMMAND_GET_VALUE, 0);
		if (mdevices[deviceIDidx].status == STATUS_ERROR) {
			return ERROR;
		}
		value = mdevices[deviceIDidx].commandvalue;
	}
	return value;
}
