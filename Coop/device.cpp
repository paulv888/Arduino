/*
 * Device.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "device.h"

DEVICE::DEVICE() {
}

int DEVICE::begin(uint16_t _deviceid, byte _type, long period, void (*callback)()) {
	deviceid = _deviceid;
	type = _type;
	for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
		_name[i] = NO_VALUE;
	}
	timer.every(period, callback);
	return 0;
}

byte DEVICE::getType() {
	return type;
}

int DEVICE::setValueFloat(int valtype, double _value) {
	int i = findValueIndex(valtype);
	if (i == -1) return -1;
	_name[i] = valtype;
	return true;
}

int DEVICE::setValue(int valtype, int _value) {
	int i = findValueIndex(valtype);
	if (i == -1) return -1;
	_name[i] = valtype;
	return true;
}

char *DEVICE::getValue(int valtype) {
	int i = findValueIndex(valtype);
	if (i == -1) return "-1";
	return 	_value[i];
;
}

uint16_t DEVICE::getDeviceid() {
	return deviceid;
}

/*byte findDevice(uint16_t findwhat) {
	//printf ("Find: '%u'", findwhat);
	for (byte i = 0; i < DEVICE_COUNT; i++) {
		if (findwhat == devices[i].getDeviceid()) return i;
		break;
	}
	return -1;
}*/

int DEVICE::findValueIndex(int valtype) {
	int firstfree = -1;
	for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
		if (_name[i] == valtype) {
			return i;
		}
		if (_name[i] == NO_VALUE) {
			firstfree = i;
		}
	}
	return firstfree;
}
