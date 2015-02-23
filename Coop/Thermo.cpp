/*
 * thermo.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */
#include "thermo.h"

void thermoTimer(const byte deviceIdx) {

	int isRunning;
	isRunning = digitalRead(mdevices[deviceIdx].getPin());

	mdevices[deviceIdx].readInput();
	if (mdevices[deviceIdx].status) {					// Enabled or Not
		int value;
		if ((value = mdevices[deviceIdx].commandvalue) != ERROR) {							// Could not read temp, leave as is
			if (mdevices[deviceIdx].getType() == TYPE_THERMO_COOL) {
				if (value >= EEPROMReadInt(PARAMS(deviceIdx, 1))) {						// Switch on if above set point
					digitalWrite(mdevices[deviceIdx].getPin(), HIGH);
				}
				if (value <= (EEPROMReadInt(PARAMS(deviceIdx, 1)) - EEPROMReadInt(PARAMS(deviceIdx, 2)))) {	// Switch off if below threshold
					digitalWrite(mdevices[deviceIdx].getPin(), LOW);
				}
			} 
			if (mdevices[deviceIdx].getType() == TYPE_THERMO_HEAT) {
				if (value <= EEPROMReadInt(PARAMS(deviceIdx, 1))) {						// 	Switch on if below set point
					digitalWrite(mdevices[deviceIdx].getPin(), HIGH);
				}
				if (value >= (EEPROMReadInt(PARAMS(deviceIdx, 1)) + EEPROMReadInt(PARAMS(deviceIdx, 2)))) {	// Switch off if above threshold
					digitalWrite(mdevices[deviceIdx].getPin(), LOW);
				}
			}
		}			// If Error read fall trough
	} else {
		digitalWrite(mdevices[deviceIdx].getPin(), LOW);
	}
	if (isRunning != digitalRead(mdevices[deviceIdx].getPin())) {
		mdevices[deviceIdx].setCommand(COMMAND_SET_RESULT);
		sprintf(temp, "{\"V\":\"%i\",\"R\":\"%i\",\"S\":\"%u\",\"T\":\"%u\"}", mdevices[deviceIdx].commandvalue, digitalRead(mdevices[deviceIdx].getPin()), EEPROMReadInt(PARAMS(deviceIdx, 1)), EEPROMReadInt(PARAMS(deviceIdx, 2)));
		mdevices[deviceIdx].setExtData(temp);
		postMessage(deviceIdx);
	}
	// nothing to do
}

