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
}

void Device::begin(const char* _name, const int _deviceid, uint8_t _type, uint8_t _Idx, const long period, void (*_init)(const byte), void (*_callback)(), uint8_t (*_commandHandler)(const uint8_t, const int, const int)) {
	deviceid = _deviceid;
	type = _type;
	index = _Idx;
	strncpy (name, _name, MAX_NAME_LEN);

	commandHandler = _commandHandler;
	Serial.print("_Idx ");
	Serial.println(_Idx);
	if (_init != NULL ) _init(_Idx);


	if (period > 0) {
		timer.every(period, _callback);	// Only for polling mdevices
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

void Device::setInOut(const uint8_t _value) {
	inout = _value;
}
char *Device::getInOut() {
	sprintf(temp, "%i", inout);
	return 	temp;
}

void Device::setExtData(const char *_value) {
	strncpy (extdata, _value, MAX_EXT_DATA);
}
char *Device::getExtData() {
	return 	extdata;
}


int Device::getDeviceid() {
	return deviceid;
}
