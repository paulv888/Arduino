/*
 * Device.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "device.h"
#ifndef NULL
#define NULL   ((void *) 0)
#endif

Device::Device() {
	commandvalue = NO_VALUE;
}

void Device::begin(const char* _name, const int _deviceid, uint8_t _Idx, const long period, void (*_init)(const byte), void (*_callback)(), uint8_t (*_commandHandler)(const uint8_t, const int, const int)) {
	deviceid = _deviceid;
	index = _Idx;
	commandvalue = NO_VALUE;

    if (name == NULL) {
    	name = (char*)malloc(strlen(_name)+1);
    }
	strncpy (name, _name, MAX_NAME_LEN-1);

	commandHandler = _commandHandler;
	if (_init != NULL ) _init(_Idx);


	if (period > 0) {
		if (timer.every(period, _callback) < 0) {		// Only for polling mdevices
//			showStatus(TIMER_ERROR, _Idx);
			if (DEBUG_DEVICE) Serial.print("ETMR");
			if (DEBUG_DEVICE) Serial.println(_Idx);
		}
	}
}

const char *Device::getName() {
	return name;
}

byte Device::getIndex() {
	return index;
}

void Device::setStatus(const int _value) {
	status = _value;
}
char *Device::getStatus() {
	sprintf(temp, "%i", status);
	return 	temp;
}

void Device::setCommand(const int _value) {
	command = _value;
}
char *Device::getCommand() {
	sprintf(temp, "%i", command);
	return 	temp;
}

void Device::setValue(const int _value) {
	commandvalue = _value;
}
char *Device::getValue() {
	sprintf(temp, "%i", commandvalue);
	return 	temp;
}

void Device::setPin(const byte _value) {
	pin = _value;
}

byte Device::getPin() {
	return 	pin;
}

void Device::setExtData(const char *_value) {
    if (extdata == NULL) {
    	extdata = (char*)malloc(MAX_EXT_DATA);
		if (DEBUG_DEVICE) Serial.println("M-Ext");
    }
	strncpy (extdata, _value, MAX_EXT_DATA);
}
char *Device::getExtData() {
    if (extdata == NULL) {
    	return 	"";
    }
	return 	extdata;
}


int Device::getDeviceid() {
	return deviceid;
}
