/*
 * Device.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "device.h"

DEVICE::DEVICE() {
}

int DEVICE::begin(uint16_t _deviceid, byte _type) {
	deviceid = _deviceid;
	type = _type;
	return 0;
}


byte DEVICE::getType() {
	return type;
}

double DEVICE::getUptime() {
	return uptime / 1000;
}

double DEVICE::getInternalTemp() {
	return internaltemp;
}

void DEVICE::setCommand(byte _command) {
	command = _command;
}

void DEVICE::setValue(byte _myvalue) {
	value = _myvalue;
}

byte DEVICE::getCommand() {
	return command;
}

int DEVICE::getFreeMemory() {
	return freememory;
}

byte DEVICE::getData(void) {
	return data;
}

uint16_t DEVICE::getDeviceid() {
	return deviceid;
}


