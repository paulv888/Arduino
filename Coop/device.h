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

class Device {
public:
	Device();
	void begin(const char* _name, const int _deviceid,  uint8_t _Idx, const long period, void (*_init)(const byte), void (*_callback)(), byte (*_commandHandler)(const byte,const int,const int));
	const char *getName();
	int getDeviceid();
	byte getIndex();
	void setStatus(const int _value);
	char *getStatus();
	void setCommand(const int _value);
	char *getCommand();
	void setValue(const int _value);
	char *getValue();
	void setPin(const byte _value);
	byte getPin();
	void setExtData(const char _value[]);
	char *getExtData();
	byte (*commandHandler)(const byte, const int, const int) ;
	int commandvalue;

protected:
	char *name;
	char *extdata;
	int deviceid;
	byte index;
	int status;
	int command;
	byte pin;
	char temp[10];
};
extern Timer timer;

#endif /* DEVICE_H_ */
