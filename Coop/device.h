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
	void begin(const char* _name, const int _deviceid, uint8_t _type, const long period, void (*_init)(), void (*_callback)(), uint8_t (*_commandHandler)(const uint8_t,const int,const int));
	const char *getName();
	int getDeviceid();
	uint8_t setValueFloat(const uint8_t _valuetype, const double _value);
	uint8_t setValueStr(const uint8_t _valtype, const char _value[]);
	uint8_t setValueInt(const uint8_t _valuetype, const int _value);
	uint8_t setValueUL(const uint8_t _valuetype, const unsigned long _value);
	char *getValue(const uint8_t _valuetype);
	char *getValuebyInd(const uint8_t Idx);
	uint8_t getValueTypebyInd(const uint8_t Idx);
	uint8_t (*commandHandler)(const uint8_t, const int, const int) ;

protected:
	int deviceid;
	uint8_t type;
	bool status;
	char* name;
	uint8_t valtype[MAX_NUMBER_OF_VALUES];
	char  *value[MAX_NUMBER_OF_VALUES];
	uint8_t findValueIndex(const uint8_t _valtype);
};
extern Timer timer;

#endif /* DEVICE_H_ */
