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

#define MAX_NUMBER_OF_VALUES 10
class Device {
public:
	Device();
	void begin(const char* _name, int _deviceid, byte _type, long period, void (*callback)(), int (*_commandHandler)(int,int,int));
	void setName(char _name[]);
	const char *getName();
	int getDeviceid();
	int setValueFloat(int _valuetype, double _value);
	int setValueInt(int _valuetype, int _value);
	int setValueUL(int _valuetype, unsigned long _value);
	char *getValue(int _valuetype);
	char *getValuebyInd(int Idx);
	int getValueTypebyInd(int Idx);
	int (*commandHandler)(int, int, int) ;

protected:
	int deviceid;
	byte type;
	bool status;
	char* name;
	int valtype[MAX_NUMBER_OF_VALUES];
	char  *value[MAX_NUMBER_OF_VALUES];
	int findValueIndex(int _valtype);
};
extern Timer timer;

#endif /* DEVICE_H_ */
