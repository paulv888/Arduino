/*
 * Led.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: pvloon
 */
#include "Led.h"

void showStatus(byte Error, byte deviceIDidx) {
	char a[MAX_EXT_DATA];
	switch (Error) {
	case INFO_NORMAL:
		timer.oscillate(LED_PIN, HIGH, 500, 2, 3);
		break;
	case INFO_CALIBRATING:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 3);
		break;
	case SENSOR_ERROR:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 4);
		if (DEBUG_DEVICE_HAND) Serial.println("ERR!");
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(ERROR);
		sprintf(a, "{\"E\" : \"%i\", \"M\" : \"%s\" , \"D\" : \"%i\"}", SENSOR_ERROR, "Sensor error on Dev: ", mdevices[deviceIDidx].getDeviceid());
    	mdevices[deviceIDidx].setExtData(a);
		postMessage(deviceIDidx);
		break;
	}
}

