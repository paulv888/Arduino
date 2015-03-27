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
//#include <dht.h>
//#include "Door.h"
#include "configuration.h"
#include "Utils.h"
#include "MemoryFree.h"
#include "SharedCommands.h"
#include "Devicehandler.h"

class Device {
public:
	void begin(const int,  const int);
	void setCheckTimer(const long, void (*)(const byte));
	void setCheckTimer( const int );
	int getCheckType();

	void setReportType(byte);
	byte getReportType();
	int getDeviceID();
	void setStatus(const int);
	char *getStatus();
	void setCommand(const int);
	char *getCommand();
	void setValue(const int);
	char *getValue();
	void setOnOff(const int);
	void readInput();
	void setPin(const byte);
	byte getPin();
	void setExtData(const char[]);
	char *getExtData();
	void setInput(const byte);
	byte getInput();
	void setType(const byte);
	byte getType();
	int commandValue;
	int status;

protected:
	bool checkCommandValue();
	void checkStatus();
	char *extdata;
	int deviceID;
	byte deviceIdx;
	int commandID;
	byte pin;
	byte inputIdx;
	byte type;
	int previousStatus;
	int previousCommandValue;
	byte reportType;
	byte checkType;
};
extern Timer timer;
extern char temp[MAX_STRING_LEN];
extern SoftwareSerial dbg;
#endif /* DEVICE_H_ */

