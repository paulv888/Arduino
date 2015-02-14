/*
 * thermo.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */
#include "thermo.h"

void thermoCallbackT() {
	thermoCallback (THERMO_IDX);
	thermoCallback (AUTO_FAN_IDX);
}

void thermoCallback(const byte deviceIDidx) {

	int isRunning;
	isRunning = digitalRead(mdevices[deviceIDidx].getPin());

	if (mdevices[deviceIDidx].status) {				// Enabled or Not
		int value;
		if ((value = ReadTemp(mdevices[deviceIDidx].getInput())) != ERROR) {
			if (mdevices[deviceIDidx].getType() == TYPE_THERMO_HEAT) {
				if (value < EEPROMReadInt(deviceIDidx * 6 + 0)) {					// below set point
					if (!isRunning) {				// switch on
						digitalWrite(mdevices[deviceIDidx].getPin(), HIGH);
						deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, true);
					}
				} else {
					if (value >= (EEPROMReadInt(deviceIDidx * 6 + 0) - EEPROMReadInt(deviceIDidx * 6 + 2))) {	// above set point plus threshold
						if (isRunning) {				// switch off
							digitalWrite(mdevices[deviceIDidx].getPin(), LOW);
							deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, true);
						}

					}

				}
			} else {
				if (value > EEPROMReadInt(deviceIDidx * 6 + 0)) {					// below set point
					if (!isRunning) {				// switch on
						digitalWrite(mdevices[deviceIDidx].getPin(), HIGH);
						deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, true);
					}
				} else {
					if (value <= (EEPROMReadInt(deviceIDidx * 6 + 0) - EEPROMReadInt(deviceIDidx * 6 + 2))) {	// above set point plus threshold
						if (isRunning) {				// switch off
							digitalWrite(mdevices[deviceIDidx].getPin(), LOW);
							deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, true);
						}

					}

				}
			}
		}			// If Error read fall trough
	} else {
		if (isRunning) {				// switch off
			digitalWrite(mdevices[deviceIDidx].getPin(), LOW);
			deviceCommandHandler(deviceIDidx, COMMAND_SET_RESULT, true);
		}
	}
// nothing to do
}

