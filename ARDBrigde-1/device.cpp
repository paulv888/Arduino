/*
 * Device.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "device.h"
#include "device_config.h"

DEVICE::DEVICE() {
}

int DEVICE::begin(uint16_t _deviceid, int _type, uint16_t _node_address) {
	deviceid = _deviceid;
	type = _type;
	node_address = _node_address;
	return 0;
}

int DEVICE::getType() {
	return type;
}

void DEVICE::setStatus(bool _status) {
	status = _status;
	ageMessage = millis();
}

bool DEVICE::getStatus() {
	return status;
}

uint16_t DEVICE::getDeviceid() {
	return deviceid;
}

uint16_t DEVICE::getNodeAddress(){
	return node_address;
}
int DEVICE::getCommand(){
	return command;
}
void DEVICE::setCommand(int _command){
	command=_command;
}

int DEVICE::ageLastMessage(){
	return millis() - ageMessage;
}
