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
#include "dht.h"
#include "Devicehandler.h"

class Device {
public:
	void begin(const char* _name, const int _deviceid,  uint8_t _Idx, const long period, void (*_callback)(), byte (*_commandHandler)(const byte,const int,const int));
	const char *getName();
	int getDeviceid();
	byte getIndex();
	void setStatus(const int _value);
	char *getStatus();
	void setCommand(const int _value);
	char *getCommand();
	void setValue(const int _value);
	char *getValue();
	void setOnOff(const int commandID);
	void readInput();
	void setPin(const byte _value);
	byte getPin();
	void setExtData(const char _value[]);
	char *getExtData();
	void setInput(const byte _value);
	byte getInput();
	void setType(const byte _value);
	byte getType();
	byte (*commandHandler)(const byte, const int, const int) ;
	int commandvalue;
	int status;

protected:
	char *name;
	char *extdata;
	int deviceid;
	byte index;
	int command;
	byte pin;
	byte inputIdx;
	byte type;
};
extern Timer timer;
#endif /* DEVICE_H_ */
