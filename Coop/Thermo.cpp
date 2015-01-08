/*
 * thermo.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */
#include "thermo.h"

bool runHeater = false;
/*
 *
 */

void tHndlrValues(const byte deviceIDidx, const int commandID) {
	char a[MAX_EXT_DATA];
	mdevices[deviceIDidx].setCommand(commandID);
	mdevices[deviceIDidx].setValue(ReadTemp(NTC_0_IDX));
	sprintf(a, "{\"C\":\"%i\",\"R\":\"%i\",\"S\":\"%u\",\"T\":\"%u\"}", mdevices[deviceIDidx].commandvalue, !digitalRead(RELAY_HEAT_PIN), EEPROMReadInt(THERMO_SET_ADDRESS), EEPROMReadInt(THERMO_THRESHOLD_ADDRESS));
	mdevices[deviceIDidx].setExtData(a);
}

void thermoInit(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("thermoI");
	if (DEBUG_DEVICE_HAND) Serial.println(deviceIDidx);
	pinMode(RELAY_HEAT_PIN, OUTPUT);
	thermoHandler(deviceIDidx, COMMAND_ON, 0);
	postMessage(deviceIDidx);
}

void thermoCallbackT() {
	thermoCallback (THERMO_IDX);
}

void thermoCallback(const byte deviceIDidx) {

	if (mdevices[deviceIDidx].status) {
		int value;
		value = ReadTemp(NTC_0_IDX);
		runHeater = !digitalRead(RELAY_HEAT_PIN);
		if (value > EEPROMReadInt(THERMO_SET_ADDRESS)) {					// below set point
			if (!runHeater) {				// switch on
				runHeater = true;
				digitalWrite(RELAY_HEAT_PIN, LOW);
				thermoHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
			}
		} else {
			if (value <= (EEPROMReadInt(THERMO_SET_ADDRESS) - EEPROMReadInt(THERMO_THRESHOLD_ADDRESS))) {	// above set point plus threshold
				if (runHeater) {				// switch off
					runHeater = false;
					digitalWrite(RELAY_HEAT_PIN, HIGH);
					thermoHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
				}

			}

		}
	} else {
		if (runHeater) {				// switch off
			runHeater = false;
			digitalWrite(RELAY_HEAT_PIN, HIGH);
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
	case COMMAND_OFF:												// Closed
		mdevices[deviceIDidx].setStatus(STATUS_OFF);
		tHndlrValues(deviceIDidx, commandID);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_1:
		EEPROMWriteInt(THERMO_SET_ADDRESS, commandvalue);
		tHndlrValues(deviceIDidx, commandID);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_2:
	    EEPROMWriteInt(THERMO_THRESHOLD_ADDRESS, commandvalue);
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
	int value;
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
		value = value + EEPROMReadInt(NTC_0_ADDRESS);
	}
	return value;
}
