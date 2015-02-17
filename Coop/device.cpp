/*
 * Device.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "device.h"

Dht dht;

int ReadTemp(const byte deviceIdx) {
	mdevices[deviceIdx].readInput();
	if (mdevices[deviceIdx].status == STATUS_ERROR) {
		return ERROR;
	}
	return mdevices[deviceIdx].commandvalue;
}

void Device::begin(const int _deviceid, const int _deviceIdx) {
/*!
*		Initializes a device. 
*/
	deviceid = _deviceid;
	deviceIdx = _deviceIdx;
	reportType = REPORT_NONE;
	checkType = CHECK_NONE;

//    if (name == NULL) {
//    	name = (char*)malloc(strlen(_name)+1);
//    }
//    strncpy (name, _name, MAX_NAME_LEN-1);

	switch (type) {
	case TYPE_ARDUINO:
		deviceCommandHandler(deviceIdx, COMMAND_PING, true);
		pinMode(getPin(), OUTPUT);
		break;
	case TYPE_DIGITAL_IO:
		pinMode(getPin(), OUTPUT);
		deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
		break;
	case TYPE_ANALOG_IN:
	case TYPE_NTC:
		deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
		break;
	case TYPE_DHT22:
		deviceCommandHandler(deviceIdx, COMMAND_PING, true);
		break;
	case TYPE_THERMO_HEAT:
	case TYPE_THERMO_COOL:
		pinMode(getPin(), OUTPUT);
		deviceCommandHandler(deviceIdx, COMMAND_ON, true);
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
}

void Device::setCheckTimer( const int _type ) {
	checkType = _type;
}

void Device::setCheckTimer( const long _period, void (*_function)(const byte)) {

	checkType = CHECK_OWN_TIMER;
	if (_period > 0) {
		if (timer.every(_period, _function, deviceIdx) < 0) {		// Only for polling mdevices
			showStatus(TIMER_ERROR, deviceIdx);
//			Serial.print("ETMR");
//			Serial.println(deviceIdx);
		}
	}
}

int Device::getCheckType() {
	return checkType;
}
void Device::setReportType( byte _reportType) {
	reportType = _reportType;
}

byte Device::getReportType( ) {
	return reportType;
}

void Device::readInput() {
/*!
*		Reads current input value of device	
*/
	commandvalue = 0;

	switch (type) {
	case TYPE_DIGITAL_IO:
		status = digitalRead(getPin());
		break;
	case TYPE_ANALOG_IN:
	case TYPE_NTC:
		commandvalue = analogRead(getPin());
		if (type == TYPE_NTC) commandvalue = 1024 - commandvalue;
		if (EEPROMReadInt(PARAMS(deviceIdx, 1)) == FFFF) {				// No setpoint set
			status = STATUS_UNKNOWN;
			sprintf(temp, "{\"V\":\"%i\"}", commandvalue);
			setExtData(temp);
		} else {
			prev_status = status;
			if (commandvalue > EEPROMReadInt(PARAMS(deviceIdx, 1))) {												// below set point
				status = STATUS_ON;
			} else if (commandvalue <= (EEPROMReadInt(PARAMS(deviceIdx, 1)) - EEPROMReadInt(PARAMS(deviceIdx, 2)))) {		// above set point plus threshold
				status = STATUS_OFF;
			}
			sprintf(temp, "{\"V\":\"%i\",\"S\":\"%u\",\"T\":\"%u\"}", commandvalue, EEPROMReadInt(PARAMS(deviceIdx, 1)), EEPROMReadInt(PARAMS(deviceIdx, 2)));
			setExtData(temp);
			if (prev_status != status) {
				deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
			}
		}
		break;
	case TYPE_DHT22:
		byte chk;
		status = STATUS_ERROR;
		chk = dht.read(getPin());
		switch (chk) {
		case DHTLIB_OK:
			commandvalue = ((int)dht.temperature);
			int temp1;
			int temp2;
			temp1 = abs((dht.temperature - (int)dht.temperature) * 100);
			temp2 = (dht.humidity - (int)dht.humidity) * 100;
			if (EEPROMReadInt(PARAMS(deviceIdx, 1)) == FFFF) {				// No setpoint set
				status = STATUS_UNKNOWN;
				sprintf(temp, "{\"T\":\"%0d.%d\",\"H\":\"%0d.%d\"}", (int)dht.temperature, temp1, (int)dht.humidity, temp2);
			} else {
				prev_status = status;
				if (commandvalue > EEPROMReadInt(PARAMS(deviceIdx, 1))) {												// below set point
					status = STATUS_ON;
				} else if (commandvalue <= (EEPROMReadInt(PARAMS(deviceIdx, 1)) - EEPROMReadInt(PARAMS(deviceIdx, 2)))) {		// above set point plus threshold
					status = STATUS_OFF;
				}
				sprintf(temp, "{\"T\":\"%0d.%d\",\"H\":\"%0d.%d\",\"S\":\"%u\"}", (int)dht.temperature, temp1, (int)dht.humidity, temp2, EEPROMReadInt(PARAMS(deviceIdx, 1)));
				if (prev_status != status) {
					deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
				}
			}
			setExtData(temp);
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
		sprintf(temp, "{\"V\":\"%i\",\"R\":\"%i\",\"S\":\"%u\",\"T\":\"%u\"}", commandvalue, digitalRead(getPin()), EEPROMReadInt(PARAMS(deviceIdx, 1)), EEPROMReadInt(PARAMS(deviceIdx, 2)));
		setExtData(temp);
		break;
	case TYPE_AUTO_DOOR:
		prev_status = status;
		if (!digitalRead(TOP_SWITCH_PIN)) {
			status = STATUS_ON;
		} else if (!digitalRead(BOTTOM_SWITCH_PIN)) {
			status = STATUS_OFF;
		} else {
			status = STATUS_UNKNOWN;
		}
		
		sprintf(temp, "{\"T\":\"%i\",\"B\":\"%i\",\"P\":\"%i\",\"D\":\"%i\"}", digitalRead(TOP_SWITCH_PIN), digitalRead(BOTTOM_SWITCH_PIN), digitalRead(POWER_RELAY_PIN), digitalRead(DIRECTION_RELAY_PIN));
		setExtData(temp);

		if (prev_status != status) {
			if (DEBUG_DEVICE_HAND) Serial.print("Change ");
			if (DEBUG_DEVICE_HAND) Serial.println(status);
			deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
		}
		break;
	case TYPE_ARDUINO:
		prev_status = status;
		status = digitalRead(getPin());
		sprintf(temp, "{\"M\" : \"%lu\", \"U\" : \"%lu\"}", check_mem(), millis()/1000);
		setExtData(temp);
		if (prev_status != status) {
			deviceCommandHandler(deviceIdx, COMMAND_SET_RESULT, true);
		}
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
	case TYPE_NTC:
		break;
	case TYPE_DHT22:
		break;
	case TYPE_THERMO_HEAT:
	case TYPE_THERMO_COOL:
		if (commandID == COMMAND_ON) status = STATUS_ON;
		if (commandID == COMMAND_OFF) status = STATUS_OFF;
		break;
	case TYPE_AUTO_DOOR:
		doorOnOff(deviceIdx, commandID);
		break;
	default:
		break;
	}
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
    	extdata = (char*)malloc(MAX_STRING_LEN);
		if (DEBUG_DEVICE) Serial.println("M-Ext");
    }
	strncpy (extdata, _value, MAX_STRING_LEN);
}
char *Device::getExtData() {
    if (extdata == NULL) {
    	return 	"";
    }
	return 	extdata;
}


int Device::getDeviceID() {
	return deviceid;
}

