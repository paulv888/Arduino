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
	int begin(uint16_t _deviceid, byte _type, uint16_t _node_address, int max_sensors);
	byte getType();
	uint16_t getNodeAddress();
	uint16_t getDeviceid();
	void setUptime(unsigned long _uptime);
	double getUptime();
	void setInternalTemp(unsigned long internaltemp);
	double getInternalTemp();
	void setFreeMemory(int _freememory);
	int getFreeMemory();
	void setData(byte _data);
	byte getData();
	void setCommand(byte _command);
	byte getCommand();
	void setMaxSensors(int _maxSensor);
	int getMaxSensors();
	void setSensorValue(byte _sensorid, int _Value, unsigned int _Age);
	unsigned int getSensorAge(byte _sensorid);
	int getSensorValue(byte _sensorid);

	unsigned int rfTransmitPackets;
	unsigned int rfReceivePackets;
	unsigned int rfSendErrors;

	void clearValues();
	byte needpost; 												// received new values



protected:
	uint16_t deviceid;
	byte type;
	bool status;
	byte command;
	unsigned long uptime;
	int freememory;
	uint16_t node_address;
	byte data;
	double internaltemp;
	byte maxSensor;
	int* sensorValue;
	unsigned int* sensorValueAge;
};

#endif /* DEVICE_H_ */
