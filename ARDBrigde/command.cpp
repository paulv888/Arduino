/*
 * command.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: pvloon
 */
// Do not remove the include below
#include "command.h"

//void packCommandRFValues(uint16_t to, payLoad_Range_Values &command) ;

//payLoad_Range_Values packCommandRFValues(uint16_t to, payLoad_Range_Values &command) {

void unpackCommandRF(byte deviceidx, payLoad_DeviceStatus &command) {

	devices[deviceidx].setCommand(command.command);
	devices[deviceidx].setData(command.data);
	devices[deviceidx].setUptime(command.upTime);
	devices[deviceidx].setFreeMemory(command.freeMemory);
	if (DEBUG_COMMAND) Serial.print("Device Unpack: ");
	if (DEBUG_COMMAND) Serial.print(devices[deviceidx].getDeviceid());
	if (DEBUG_COMMAND) Serial.print(" Uptime: ");
	if (DEBUG_COMMAND) Serial.println(devices[deviceidx].getUptime());
	return;
}

