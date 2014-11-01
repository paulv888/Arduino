/*
 * Device.h
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#ifndef DEVICE_H_
#define DEVICE_H_
#include "Arduino.h"
//add your includes for the project command here
#include "configuration.h"
#include "Utils.h"
#include "MemoryFree.h"
#include "share_commands.h"
#include "InternalTemp.h"


class DEVICE {
public:
	DEVICE();
	int begin(uint16_t _deviceid, byte _type);
	byte getType();
	uint16_t getDeviceid();
	double getUptime();
	double getInternalTemp();
	int getFreeMemory();
	byte getData();
	void setCommand(byte _command);
	void setValue(byte _myvalue);
	byte getCommand();

	byte needpost; 												// received new values



protected:
	uint16_t deviceid;
	byte type;
	bool status;
	byte command;
	byte value;
	unsigned long uptime;
	int freememory;
	uint16_t node_address;
	byte data;
	double internaltemp;
};

#endif /* DEVICE_H_ */
