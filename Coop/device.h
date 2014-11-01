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
#include "Timer.h"

#define MAX_NUMBER_OF_VALUES 5

class DEVICE {
public:
	DEVICE();
	int begin(uint16_t _deviceid, byte _type);
	int begin(uint16_t _deviceid, byte _type, long period, void (*callback)());
	byte getType();
	uint16_t getDeviceid();
	int setValue(int _name, String _value);
	String getValue(int _name);
	String getValuebyIndex(int _idx);

protected:
	uint16_t deviceid;
	byte type;
	bool status;
	int _name[MAX_NUMBER_OF_VALUES];
	String _value[MAX_NUMBER_OF_VALUES];
	int findValueIndex(int _name);
	String val;
};
extern Timer timer;

#endif /* DEVICE_H_ */
