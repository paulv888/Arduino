/*
 * Device.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "device.h"
#ifndef NULL
#define NULL   ((void *) 0)
#endif

Device::Device() {
}

void Device::begin(const char* _name, const int _deviceid, uint8_t _type, const long period, void (*_init)(), void (*_callback)(), uint8_t (*_commandHandler)(const uint8_t, const int, const int)) {
	deviceid = _deviceid;
	type = _type;
	if (DEBUG_MEMORY) {
		Serial.print("bg 1 ");
		Serial.print((int)name);
		Serial.print("  ");
		Serial.println(freeMemory());
	}
	name = malcpy(_name);			// PVTODO:: Do not use heap, per init vars. Put in progmem?

	if (DEBUG_MEMORY) {
		Serial.print("bg 2 ");
		Serial.print((int)name);
		Serial.print("  ");
		Serial.println(freeMemory());
	}
	commandHandler = _commandHandler;
	if (_init != NULL ) _init();

	if (DEBUG_MEMORY) {
		Serial.print("bg 3 ");
		Serial.println(freeMemory());
	}

	for (uint8_t i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
		valtype[i] = ERROR;
	}

	if (DEBUG_MEMORY) {
		Serial.print("bg 4 ");
		Serial.println(freeMemory());
	}

	if (period > 0) {
		timer.every(period, _callback);	// Only for polling mdevices
	}
}

const char *Device::getName() {
	return name;
}

uint8_t Device::setValueFloat(const uint8_t _valtype, const double _value) {
	uint8_t i = findValueIndex(_valtype);
	if (i == ERROR) return ERROR;
	valtype[i] = _valtype;

	char a[10];

	int temp1 = (_value - (int)_value) * 100;
	sprintf(a, "%0d.%d", (int)_value, temp1);

	free (value[i]);
	value[i] = 	malcpy(a);

	if (DEBUG_DEVICE) {
		Serial.println();
		for (uint8_t i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
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

uint8_t Device::setValueStr(const uint8_t _valtype, const char _value[]) {
	uint8_t i = findValueIndex(_valtype);
	if (i == ERROR) return ERROR;

	valtype[i] = _valtype;

	free (value[i]);

	value[i] = 	malcpy(_value);

	if (DEBUG_DEVICE) {
		Serial.println();
		for (uint8_t i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
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

uint8_t Device::setValueUL(const uint8_t _valtype, const unsigned long _value) {
	uint8_t i = findValueIndex(_valtype);
	if (i == ERROR) return ERROR;
	valtype[i] = _valtype;

	char a[10];
	sprintf( a ,"%lu", _value );

	free (value[i]);
	value[i] = 	malcpy(a);

	if (DEBUG_DEVICE) {
		Serial.println();
		for (uint8_t i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
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

uint8_t Device::setValueInt(const uint8_t _valtype, const int _value) {
	uint8_t i = findValueIndex(_valtype);
	if (i == ERROR) return ERROR;
	valtype[i] = _valtype;

	if (DEBUG_MEMORY) {
		Serial.print("in 1 ");
		Serial.println(freeMemory());
	}

	char a[10];
	sprintf( a ,"%i", _value );
	if (DEBUG_MEMORY) {
		Serial.print("in 2 ");
		Serial.println(freeMemory());
	}

	free (value[i]);

	if (DEBUG_MEMORY) {
		Serial.print("in 3 ");
		Serial.println(freeMemory());
	}
	value[i] = 	malcpy(a);

	if (DEBUG_MEMORY) {
		Serial.print("in 4 ");
		Serial.println(freeMemory());
	}

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

char *Device::getValue(const uint8_t _valtype) {
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
	uint8_t i = findValueIndex(_valtype);
	if (i == ERROR) return "ERROR";
	return 	value[i];
}

char *Device::getValuebyInd(const uint8_t Idx) {
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

uint8_t Device::getValueTypebyInd(const uint8_t Idx) {
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

uint8_t Device::findValueIndex(const uint8_t _valtype) {
	int firstfree = ERROR;
	for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
		if (valtype[i] == _valtype) {
			return i;
		}
		if (valtype[i] == ERROR) {
			firstfree = i;
			break;
		}
	}
	return firstfree;
}
