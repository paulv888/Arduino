/*
 * Device.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "device.h"

DEVICE::DEVICE() {
}

int DEVICE::begin(uint16_t _deviceid, byte _type, uint16_t _node_address, int max_sensors) {
	deviceid = _deviceid;
	type = _type;
	node_address = _node_address;
	setMaxSensors(max_sensors);
	return 0;
}

void DEVICE::clearValues() {
	for (int sensorid = 0; sensorid < getMaxSensors(); sensorid++) {
		sensorValue[sensorid] = 0;
		sensorValueAge[sensorid] = 0;
	}
	uptime = 0;
	freememory = 0;
}

byte DEVICE::getType() {
	return type;
}

void DEVICE::setUptime(unsigned long _uptime) {
	uptime = _uptime;
}

double DEVICE::getUptime() {
	return uptime / 1000;
}

void DEVICE::setInternalTemp(unsigned long _internaltemp) {
	internaltemp = _internaltemp;
}

double DEVICE::getInternalTemp() {
	return internaltemp;
}
void DEVICE::setCommand(byte _command) {
	command = _command;
}
byte DEVICE::getCommand() {
	return command;
}
void DEVICE::setFreeMemory(int _freememory) {
	freememory = _freememory;
}
int DEVICE::getFreeMemory() {
	return freememory;
}

void DEVICE::setData(byte _data) {
	data = _data;
}

byte DEVICE::getData(void) {
	return data;
}

void DEVICE::setSensorValue(byte _sensorid, int _Value, unsigned int _Age) {
	sensorValue[_sensorid] = _Value;
	sensorValueAge[_sensorid] = _Age;
}

unsigned int DEVICE::getSensorAge(byte _sensorid) {
	return sensorValueAge[_sensorid] / 1000;
}

int DEVICE::getSensorValue(byte _sensorid) {
	return sensorValue[_sensorid];
}

void DEVICE::setMaxSensors(int _maxSensor) {
	sensorValue = new int[_maxSensor];
	sensorValueAge = new unsigned int[_maxSensor];
	for (int i = 0; i < _maxSensor; i++) {
		sensorValue[i] = 0; // Initialize all elements to zero.
		sensorValueAge[i] = 0; // Initialize all elements to zero.
	}
	maxSensor = _maxSensor;
}

int DEVICE::getMaxSensors(void) {
	return maxSensor;
}

uint16_t DEVICE::getDeviceid() {
	return deviceid;
}

uint16_t DEVICE::getNodeAddress() {
	return node_address;
}

