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

DHT dht;
char a[MAX_EXT_DATA];
char temp[10];

void Device::begin(const char* _name, const int _deviceid, uint8_t _Idx, const long _period, void (*_polfunction)()) {
	deviceid = _deviceid;
	index = _Idx;

    if (name == NULL) {
    	name = (char*)malloc(strlen(_name)+1);
    }
    strncpy (name, _name, MAX_NAME_LEN-1);

	switch (type) {
	case TYPE_ARDUINO:
		deviceCommandHandler(_Idx, COMMAND_PING, true);
		pinMode(getPin(), OUTPUT);
		break;
	case TYPE_DIGITAL_IO:
		pinMode(getPin(), OUTPUT);
		break;
	case TYPE_ANALOG_IN:
		deviceCommandHandler(_Idx, COMMAND_GET_VALUE, false);
		break;
	case TYPE_THERMO_HEAT:
	case TYPE_THERMO_COOL:
		pinMode(getPin(), OUTPUT);
		deviceCommandHandler(_Idx, COMMAND_ON, true);
		break;
	case TYPE_AUTO_DOOR:
		pinMode(POWER_RELAY_PIN, OUTPUT);
		pinMode(DIRECTION_RELAY_PIN, OUTPUT);

		pinMode(TOP_SWITCH_PIN, INPUT);
		digitalWrite(TOP_SWITCH_PIN, HIGH); 	// connect internal pull-up

		pinMode(BOTTOM_SWITCH_PIN, INPUT);
		digitalWrite(BOTTOM_SWITCH_PIN, HIGH); 	// connect internal pull-up
		break;
	}

	if (_period > 0) {
		if (timer.every(_period, _polfunction) < 0) {		// Only for polling mdevices
//			showStatus(TIMER_ERROR, _Idx);
			if (DEBUG_DEVICE) Serial.print("ETMR");
			if (DEBUG_DEVICE) Serial.println(_Idx);
		}
	}
}

void Device::readInput() {

	commandvalue = 0;

	switch (type) {
	case TYPE_DIGITAL_IO:
		status = digitalRead(getPin());
		break;
	case TYPE_ANALOG_IN:
		commandvalue = analogRead(getPin());
		if (EEPROMReadInt(index * 6 + 0) == FFFF) {				// No setpoint set
			status = STATUS_UNKNOWN;
			sprintf(a, "{\"V\":\"%i\"}", commandvalue);
			setExtData(a);
		} else {
			if (commandvalue > EEPROMReadInt(index * 6 + 0)) {												// below set point
				status = STATUS_ON;
			} else if (commandvalue <= (EEPROMReadInt(index * 6 + 0) - EEPROMReadInt(index * 6 + 2))) {		// above set point plus threshold
				status = STATUS_OFF;
			}
			sprintf(a, "{\"V\":\"%i\",\"S\":\"%u\",\"T\":\"%u\"}", commandvalue, EEPROMReadInt(index * 6 + 0), EEPROMReadInt(index * 6 + 2));
			setExtData(a);
		}
		break;
	case TYPE_DHT22:
		byte chk;
		setStatus(STATUS_ERROR);
		chk = dht.read(getPin());
		switch (chk) {
		case DHTLIB_OK:
			commandvalue = ((int)dht.temperature);
			int temp1;
			int temp2;
			temp1 = (dht.temperature - (int)dht.temperature) * 100;
			temp2 = (dht.humidity - (int)dht.humidity) * 100;
			if (EEPROMReadInt(index * 6 + 0) == FFFF) {				// No setpoint set
				status = STATUS_UNKNOWN;
				sprintf(a, "{\"T\":\"%0d.%d\",\"H\":\"%0d.%d\"}", (int)dht.temperature, temp1, (int)dht.humidity, temp2);
			} else {
				if (commandvalue > EEPROMReadInt(index * 6 + 0)) {												// below set point
					status = STATUS_ON;
				} else if (commandvalue <= (EEPROMReadInt(index * 6 + 0) - EEPROMReadInt(index * 6 + 2))) {		// above set point plus threshold
					status = STATUS_OFF;
				}
				sprintf(a, "{\"T\":\"%0d.%d\",\"H\":\"%0d.%d\",\"S\":\"%u\"}", (int)dht.temperature, temp1, (int)dht.humidity, temp2, EEPROMReadInt(index * 6 + 0));
			}
			setExtData(a);
			break;
		default:
			status= STATUS_ERROR;
			setExtData("");
			break;
		}
		break;
	case TYPE_THERMO_HEAT:
	case TYPE_THERMO_COOL:
		commandvalue = ReadTemp(getInput());
		sprintf(a, "{\"V\":\"%i\",\"R\":\"%i\",\"S\":\"%u\",\"T\":\"%u\"}", commandvalue, digitalRead(getPin()), EEPROMReadInt(index * 6 + 0), EEPROMReadInt(index * 6 + 2));
		setExtData(a);
		break;
	case TYPE_AUTO_DOOR:
		break;
	case TYPE_ARDUINO:
		status = digitalRead(getPin());
		sprintf(a, "{\"M\" : \"%lu\", \"U\" : \"%lu\"}", check_mem(), millis()/1000);
		setExtData(a);
		break;
	default:
		break;
	}
}

void Device::setOnOff(const int commandID) {

	setCommand(commandID);

	switch (type) {
	case TYPE_ARDUINO:
	case TYPE_DIGITAL_IO:
		if (commandID == COMMAND_ON) digitalWrite(getPin(), HIGH);
		if (commandID == COMMAND_OFF) digitalWrite(getPin(), LOW);
		status = digitalRead(getPin());
		break;
	case TYPE_ANALOG_IN:
		break;
	case TYPE_DHT22:
		break;
	case TYPE_THERMO_HEAT:
	case TYPE_THERMO_COOL:
		if (commandID == COMMAND_ON) status = STATUS_ON;
		if (commandID == COMMAND_OFF) status = STATUS_OFF;
		break;
	case TYPE_AUTO_DOOR:
		doorOnOff(commandID);
		break;
	default:
		break;
	}
}

const char *Device::getName() {
	return name;
}

byte Device::getIndex() {
	return index;
}

void Device::setStatus(const int _value) {
	status = _value;
}
char *Device::getStatus() {
	sprintf(temp, "%i", status);
	return 	temp;
}

void Device::setCommand(const int _value) {
	command = _value;
}
char *Device::getCommand() {
	sprintf(temp, "%i", command);
	return 	temp;
}

void Device::setValue(const int _value) {
	commandvalue = _value;
}

char *Device::getValue() {
	sprintf(temp, "%i", commandvalue);
	return 	temp;
}

void Device::setPin(const byte _value) {
	pin = _value;
}

byte Device::getPin() {
	return 	pin;
}

void Device::setInput(const byte _value) {
	inputIdx = _value;
}

byte Device::getInput() {
	return 	inputIdx;
}

void Device::setType(const byte _value) {
	type = _value;
}

byte Device::getType() {
	return 	type;
}

void Device::setExtData(const char *_value) {
    if (extdata == NULL) {
    	extdata = (char*)malloc(MAX_EXT_DATA);
		if (DEBUG_DEVICE) Serial.println("M-Ext");
    }
	strncpy (extdata, _value, MAX_EXT_DATA);
}
char *Device::getExtData() {
    if (extdata == NULL) {
    	return 	"";
    }
	return 	extdata;
}


int Device::getDeviceid() {
	return deviceid;
}
