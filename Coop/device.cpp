/*
 * Device.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "device.h"

Device::Device() {
}

void Device::begin(const char* _name, int _deviceid, byte _type, long period, void (*_callback)(), int (*_commandHandler)(int,int,int)) {
	deviceid = _deviceid;
	type = _type;
	name = malcpy(_name);
	commandHandler = _commandHandler;
	for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
		valtype[i] = NO_VALUE;
	}
	if (period > 0) {
			timer.every(period, _callback);	// Only for polling devices
	}
}

void Device::setName(char _name[]) {
	name = malcpy(_name);
	return;
}

const char *Device::getName() {
	return name;
}

int Device::setValueFloat(int _valtype, double _value) {
	int i = findValueIndex(_valtype);
	if (i == -1) return -1;
	valtype[i] = _valtype;

	char a[10];
	int temp1 = (_value - (int)_value) * 100;
	sprintf(a, "%0d.%d", (int)_value, temp1);

	free (value[i]);
	value[i] = 	malcpy(a);

	if (DEBUG_DEVICE) {
		Serial.println();
		for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
			Serial.print("**Dump Fl** ");
			Serial.print(i);
			Serial.print(" type: ");
			Serial.print(valtype[i]);
			Serial.print(" value: ");
			Serial.print(value[i]);
			Serial.println();
		}
	}
	return true;
}

int Device::setValueUL(int _valtype, unsigned long _value) {
	int i = findValueIndex(_valtype);
	if (i == -1) return -1;
	valtype[i] = _valtype;

	char a[10];
	sprintf( a ,"%lu", _value );

	free (value[i]);
	value[i] = 	malcpy(a);

	if (DEBUG_DEVICE) {
		Serial.println();
		for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
			Serial.print("**Dump UL** ");
			Serial.print(i);
			Serial.print(" type: ");
			Serial.print(valtype[i]);
			Serial.print(" value: ");
			Serial.print(value[i]);
			Serial.println();
		}
	}
	return true;

}

int Device::setValueInt(int _valtype, int _value) {
	int i = findValueIndex(_valtype);
	if (i == -1) return -1;
	valtype[i] = _valtype;

	char a[10];
	sprintf( a ,"%i", _value );

	free (value[i]);
	value[i] = 	malcpy(a);

	if (DEBUG_DEVICE) {
		Serial.println();
		for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
			Serial.print("**Dump I** ");
			Serial.print(i);
			Serial.print(" type: ");
			Serial.print(valtype[i]);
			Serial.print(" value: ");
			Serial.print(value[i]);
			Serial.println();
		}
	}


	return true;
}

char *Device::getValue(int _valtype) {
	if (DEBUG_DEVICE) {
		Serial.println();
		for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
			Serial.print("**Dump getValue** ");
			Serial.print(i);
			Serial.print(" type: ");
			Serial.print(valtype[i]);
			Serial.print(" value: ");
			Serial.print(value[i]);
			Serial.print(" _valtype ");
			Serial.print(_valtype);
			Serial.println();
		}
	}
	int i = findValueIndex(_valtype);
	if (i == -1) return "-1";
	return 	value[i];
}

char *Device::getValuebyInd(int Idx) {
	if (DEBUG_DEVICE) {
		Serial.println();
		for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
			Serial.print("**Dump by Idx** ");
			Serial.print(i);
			Serial.print(" type: ");
			Serial.print(valtype[i]);
			Serial.print(" value: ");
			Serial.print(value[i]);
			Serial.print(" Idx ");
			Serial.print(Idx);
			Serial.println();
		}
	}
	return 	value[Idx];
}

int Device::getValueTypebyInd(int Idx) {
	if (DEBUG_DEVICE) {
		Serial.println();
		for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
			Serial.print("**Dump type by Ind** ");
			Serial.print(i);
			Serial.print(" type: ");
			Serial.print(valtype[i]);
			Serial.print(" value: ");
			Serial.print(value[i]);
			Serial.print(" Idx ");
			Serial.print(Idx);
			Serial.println();
		}
	}
	return 	valtype[Idx];
}


int Device::getDeviceid() {
	return deviceid;
}

int Device::findValueIndex(int _valtype) {
	int firstfree = -1;
	for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
		if (valtype[i] == _valtype) {
			return i;
		}
		if (valtype[i] == NO_VALUE) {
			firstfree = i;
			break;
		}
	}
	return firstfree;
}
