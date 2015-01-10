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
		timer.oscillate(LED_PIN, HIGH, 500, 2, 2);
		break;
	case INFO_CALIBRATING:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 3);
		break;
	case SENSOR_ERROR:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 4);
		if (DEBUG_DEVICE_HAND) Serial.println("ERR!");
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(STATUS_ERROR);
		sprintf(a, "{\"E\":\"%i\",\"M\":\"%s\"}", SENSOR_ERROR, "Sensor read");
    	mdevices[deviceIDidx].setExtData(a);
		postMessage(deviceIDidx);
		break;
	case TIMER_ERROR:
		//timer.oscillate(LED_PIN, HIGH, 500, 50, 5);
		if (DEBUG_DEVICE_HAND) Serial.println("ERR!");
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(STATUS_ERROR);
		sprintf(a, "{\"E\":\"%i\",\"M\":\"%s\"}", TIMER_ERROR, "Timer create");
    	mdevices[deviceIDidx].setExtData(a);
		postMessage(deviceIDidx);
		break;
	case DOOR_NOT_MOVING:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 5);
		if (DEBUG_DEVICE_HAND) Serial.println("ERR!");
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(STATUS_ERROR);
		sprintf(a, "{\"E\":\"%i\",\"M\":\"%s\"}", DOOR_NOT_MOVING, "NOT moving");
    	mdevices[deviceIDidx].setExtData(a);
		postMessage(deviceIDidx);
		break;
	case DOOR_TIMEOUT:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 5);
		if (DEBUG_DEVICE_HAND) Serial.println("ERR!");
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(STATUS_ERROR);
		sprintf(a, "{\"E\":\"%i\",\"M\":\"%s\"}", DOOR_NOT_MOVING, "Timeout");
    	mdevices[deviceIDidx].setExtData(a);
		postMessage(deviceIDidx);
		break;
	}
}

