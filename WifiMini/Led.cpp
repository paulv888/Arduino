/*
 * Led.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: pvloon
 */
#include "Led.h"

void showStatus(byte Error, byte deviceIdx) {
	switch (Error) {
	case INFO_NORMAL:
		timer.oscillate(LED_PIN, HIGH, 500, 2, 2);
		break;
	case NET_ERROR:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 3);
		if (DEBUG_DEVICE_HAND) dbg.println("ERR!");
	case SENSOR_ERROR:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 4);
		if (DEBUG_DEVICE_HAND) dbg.println("ERR!");
		mdevices[deviceIdx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIdx].setStatus(STATUS_ERROR);
		sprintf(temp, "{\"E\":\"%i\",\"M\":\"%s\"}", SENSOR_ERROR, "Sensor read");
    	mdevices[deviceIdx].setExtData(temp);
		postMessage(deviceIdx);
		break;
	case TIMER_ERROR:
		//timer.oscillate(LED_PIN, HIGH, 500, 50, 5);
		if (DEBUG_DEVICE_HAND) dbg.println("ERR!");
		mdevices[deviceIdx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIdx].setStatus(STATUS_ERROR);
		sprintf(temp, "{\"E\":\"%i\",\"M\":\"%s\"}", TIMER_ERROR, "Timer create");
    	mdevices[deviceIdx].setExtData(temp);
		postMessage(deviceIdx);
		break;
	case DOOR_NOT_MOVING:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 5);
		if (DEBUG_DEVICE_HAND) dbg.println("ERR!");
		mdevices[deviceIdx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIdx].setStatus(STATUS_ERROR);
		sprintf(temp, "{\"E\":\"%i\",\"M\":\"%s\"}", DOOR_NOT_MOVING, "NOT moving");
    	mdevices[deviceIdx].setExtData(temp);
		postMessage(deviceIdx);
		break;
	case DOOR_MAX_RUNTIME:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 5);
		if (DEBUG_DEVICE_HAND) dbg.println("ERR!");
		mdevices[deviceIdx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIdx].setStatus(STATUS_ERROR);
		sprintf(temp, "{\"E\":\"%i\",\"M\":\"%s\"}", DOOR_MAX_RUNTIME, "Max Run");
    	mdevices[deviceIdx].setExtData(temp);
		postMessage(deviceIdx);
		break;
	}
}


