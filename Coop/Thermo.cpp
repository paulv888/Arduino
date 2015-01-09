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
	sprintf(a, "{\"C\":\"%i\",\"R\":\"%i\",\"S\":\"%u\",\"T\":\"%u\"}", mdevices[deviceIDidx].commandvalue, !digitalRead(RELAY_HEAT_PIN), EEPROMReadInt(deviceIDidx * 6 + 0), EEPROMReadInt(deviceIDidx * 6 + 2));
	mdevices[deviceIDidx].setExtData(a);
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
		if (mdevices[deviceIDidx].getType() == TYPE_THERMO_HEAT) {
			if (value < EEPROMReadInt(deviceIDidx * 6 + 0)) {					// below set point
				if (!isRunning) {				// switch on
					digitalWrite(mdevices[deviceIDidx].getPin(), HIGH);
					deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
				}
			} else {
				if (value >= (EEPROMReadInt(deviceIDidx * 6 + 0) - EEPROMReadInt(deviceIDidx * 6 + 2))) {	// above set point plus threshold
					if (isRunning) {				// switch off
						digitalWrite(mdevices[deviceIDidx].getPin(), LOW);
						deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
					}

				}

			}
		} else {
			if (value > EEPROMReadInt(deviceIDidx * 6 + 0)) {					// below set point
				if (!isRunning) {				// switch on
					digitalWrite(mdevices[deviceIDidx].getPin(), HIGH);
					deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
				}
			} else {
				if (value <= (EEPROMReadInt(deviceIDidx * 6 + 0) - EEPROMReadInt(deviceIDidx * 6 + 2))) {	// above set point plus threshold
					if (isRunning) {				// switch off
						digitalWrite(mdevices[deviceIDidx].getPin(), LOW);
						deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
					}

				}

			}
		}
	} else {
		if (isRunning) {				// switch off
			digitalWrite(mdevices[deviceIDidx].getPin(), LOW);
			deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, 0);
		}
	}
// nothing to do
}
