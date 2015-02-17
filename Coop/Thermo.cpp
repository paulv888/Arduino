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

	if (mdevices[deviceIdx].status) {					// Enabled or Not
		int value;
		mdevices[deviceIdx].readInput();
		if ((value = mdevices[deviceIdx].commandvalue) != ERROR) {				// Could not read temp, leave as is
			if (mdevices[deviceIdx].getType() == TYPE_THERMO_HEAT) {
				if (value < EEPROMReadInt(PARAMS(deviceIdx, 1))) {					// below set point
					if (!isRunning) {					// switch on
						digitalWrite(mdevices[deviceIdx].getPin(), HIGH);
						deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
					}
				} else {
					if (value >= (EEPROMReadInt(PARAMS(deviceIdx, 1)) - EEPROMReadInt(PARAMS(deviceIdx, 2)))) {	// above set point plus threshold
						if (isRunning) {				// switch off
							digitalWrite(mdevices[deviceIdx].getPin(), LOW);
							deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
						}

					}
				}
			} else {								// COOLING
				if (value > EEPROMReadInt(PARAMS(deviceIdx, 1))) {					// below set point
					if (!isRunning) {				// switch on
						digitalWrite(mdevices[deviceIdx].getPin(), HIGH);
						deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
					}
				} else {
					if (value <= (EEPROMReadInt(PARAMS(deviceIdx, 1)) - EEPROMReadInt(PARAMS(deviceIdx, 2)))) {	// above set point plus threshold
						if (isRunning) {				// switch off
							digitalWrite(mdevices[deviceIdx].getPin(), LOW);
							deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
						}

					}

				}
			}
		}			// If Error read fall trough
	} else {
		if (isRunning) {				// switch off
			digitalWrite(mdevices[deviceIdx].getPin(), LOW);
			deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
		}
	}
// nothing to do
}

