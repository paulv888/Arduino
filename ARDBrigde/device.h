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
#include "Utils/Utils.h"

class DEVICE {
public:
	DEVICE();
	int begin(uint16_t _deviceid, int _type, uint16_t _node_address);
	int getType();
	bool getStatus();
	void setStatus(bool _status);
	uint16_t getNodeAddress();
	int getCommand();
	void setCommand(int _command);
	int ageLastMessage();
	int sensorTemp[7];   // Current Temp
	unsigned char switchValue[6];  // Current switch Status
	uint16_t getDeviceid();

protected:
	uint16_t deviceid;
	int type;
	int status;
	int command;
	uint16_t node_address;
	long ageMessage;
};

#endif /* DEVICE_H_ */
