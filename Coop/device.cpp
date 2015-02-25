/*
 * Device.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "device.h"

Dht dht;

int readTemp(const byte deviceIdx) {
	mdevices[deviceIdx].readInput();
	if (mdevices[deviceIdx].status == STATUS_ERROR) {
		return ERROR;
	}
	return mdevices[deviceIdx].commandValue;
}


void Device::begin(const int _deviceID, const int _deviceIdx) {
/*!
*		Initializes a device. 
*/
	deviceID = _deviceID;
	deviceIdx = _deviceIdx;
	reportType = REPORT_NONE;
	checkType = CHECK_NONE;
	previousCommandValue = 0;
	previousStatus = STATUS_ERROR;

	switch (type) {
	case TYPE_ARDUINO:
		pinMode(getPin(), OUTPUT);
		readInput();
		break;
	case TYPE_DIGITAL_IO:
		pinMode(getPin(), OUTPUT);
		readInput();
		break;
	case TYPE_ANALOG_IN:
	case TYPE_NTC:
		readInput();
		break;
	case TYPE_DHT22:
		readInput();
		break;
	case TYPE_THERMO_HEAT:
	case TYPE_THERMO_COOL:
		pinMode(getPin(), OUTPUT);
		deviceCommandHandler(deviceIdx, COMMAND_ON, true);
		readInput();
		break;
	case TYPE_AUTO_DOOR:
		pinMode(POWER_RELAY_PIN, OUTPUT);
		pinMode(DIRECTION_RELAY_PIN, OUTPUT);

		pinMode(TOP_SWITCH_PIN, INPUT);
		digitalWrite(TOP_SWITCH_PIN, HIGH); 	// connect internal pull-up

		pinMode(BOTTOM_SWITCH_PIN, INPUT);
		digitalWrite(BOTTOM_SWITCH_PIN, HIGH); 	// connect internal pull-up
		readInput();
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
	commandValue = 0;
	int prevRunning;
	
	switch (type) {
	case TYPE_DIGITAL_IO:
		status = digitalRead(getPin());
		break;
	case TYPE_ANALOG_IN:
	case TYPE_NTC:
		commandValue = analogRead(getPin());
		if (type == TYPE_NTC) commandValue = 1024 - commandValue;
		if (EEPROMReadInt(PARAMS(deviceIdx, 1)) == FFFF) {				// No setpoint set
			status = STATUS_UNKNOWN;
			sprintf(temp, "{\"V\":\"%i\"}", commandValue);
			setExtData(temp);
		} else {
			if (commandValue > EEPROMReadInt(PARAMS(deviceIdx, 1))) {														// Above set point
				status = STATUS_ON;
			}
			if (commandValue <= (EEPROMReadInt(PARAMS(deviceIdx, 1)) - EEPROMReadInt(PARAMS(deviceIdx, 2)))) {		// Below set point and threshold
				status = STATUS_OFF;
			}
			sprintf(temp, "{\"V\":\"%i\",\"S\":\"%u\",\"T\":\"%u\"}", commandValue, EEPROMReadInt(PARAMS(deviceIdx, 1)), EEPROMReadInt(PARAMS(deviceIdx, 2)));
			setExtData(temp);
			checkStatus();
		}
		break;
	case TYPE_DHT22:
		byte chk;
		status = STATUS_ERROR;
		chk = dht.read(getPin());
		switch (chk) {
		case DHTLIB_OK:
			commandValue = ((int)dht.temperature);
			int temp1;
			int temp2;
			temp1 = abs((dht.temperature - (int)dht.temperature) * 100);
			temp2 = (dht.humidity - (int)dht.humidity) * 100;
			if (EEPROMReadInt(PARAMS(deviceIdx, 1)) == FFFF) {				// No setpoint set
				status = STATUS_UNKNOWN;
				sprintf(temp, "{\"T\":\"%0d.%d\",\"H\":\"%0d.%d\"}", (int)dht.temperature, temp1, (int)dht.humidity, temp2);
			} else {
				if (commandValue > EEPROMReadInt(PARAMS(deviceIdx, 1))) {												// below set point
					status = STATUS_ON;
				} 
				if (commandValue <= (EEPROMReadInt(PARAMS(deviceIdx, 1)) - EEPROMReadInt(PARAMS(deviceIdx, 2)))) {		// above set point plus threshold
					status = STATUS_OFF;
				}
				sprintf(temp, "{\"T\":\"%0d.%d\",\"H\":\"%0d.%d\",\"S\":\"%u\"}", (int)dht.temperature, temp1, (int)dht.humidity, temp2, EEPROMReadInt(PARAMS(deviceIdx, 1)));
				checkStatus();
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
		commandValue = readTemp(getInput());
		prevRunning = digitalRead(getPin());
		if (status) {					// Enabled or Not
			if (commandValue != ERROR) {							// Could not read temp, leave as is
				if (getType() == TYPE_THERMO_COOL) {
					if (commandValue >= EEPROMReadInt(PARAMS(deviceIdx, 1))) {						// Switch on if above set point
						digitalWrite(getPin(), HIGH);
					}
					if (commandValue <= (EEPROMReadInt(PARAMS(deviceIdx, 1)) - EEPROMReadInt(PARAMS(deviceIdx, 2)))) {	// Switch off if below threshold
						digitalWrite(getPin(), LOW);
					}
				} 
				if (getType() == TYPE_THERMO_HEAT) {
					if (commandValue <= EEPROMReadInt(PARAMS(deviceIdx, 1))) {						// 	Switch on if below set point
						digitalWrite(getPin(), HIGH);
					}
					if (commandValue >= (EEPROMReadInt(PARAMS(deviceIdx, 1)) + EEPROMReadInt(PARAMS(deviceIdx, 2)))) {	// Switch off if above threshold
						digitalWrite(getPin(), LOW);
					}
				}
			}			// If Error read fall trough
		} else {
			digitalWrite(mdevices[deviceIdx].getPin(), LOW);
		}
		sprintf(temp, "{\"V\":\"%i\",\"R\":\"%i\",\"S\":\"%u\",\"T\":\"%u\"}", commandValue, digitalRead(getPin()), EEPROMReadInt(PARAMS(deviceIdx, 1)), EEPROMReadInt(PARAMS(deviceIdx, 2)));
		setExtData(temp);
		if ((prevRunning != digitalRead(getPin())) || checkCommandValue()) {
			int previousCommandID = commandID;
			setCommand(COMMAND_SET_RESULT);
			postMessage(deviceIdx);
			commandID = previousCommandID;
		}	
		break;
	case TYPE_AUTO_DOOR:
		if (!digitalRead(TOP_SWITCH_PIN)) {
			status = STATUS_ON;
		} else if (!digitalRead(BOTTOM_SWITCH_PIN)) {
			status = STATUS_OFF;
		} else {
			status = STATUS_UNKNOWN;
		}
		
		sprintf(temp, "{\"T\":\"%i\",\"B\":\"%i\",\"P\":\"%i\",\"D\":\"%i\"}", digitalRead(TOP_SWITCH_PIN), digitalRead(BOTTOM_SWITCH_PIN), digitalRead(POWER_RELAY_PIN), digitalRead(DIRECTION_RELAY_PIN));
		setExtData(temp);
		checkStatus();
		break;
	case TYPE_ARDUINO:
		status = digitalRead(getPin());
		sprintf(temp, "{\"M\" : \"%lu\", \"U\" : \"%lu\"}", check_mem(), millis()/1000);
		setExtData(temp);
		checkStatus();
		break;
	default:
		break;
	}
}

void Device::setOnOff(const int _commandID) {

	commandID = _commandID;

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
	commandID = _value;
}
char *Device::getCommand() {
	sprintf(temp, "%i", commandID);
	return 	temp;
}

void Device::setValue(const int _value) {
	commandValue = _value;
}

char *Device::getValue() {
	sprintf(temp, "%i", commandValue);
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
	return deviceID;
}

bool Device::checkCommandValue(){ 
	if (EEPROMReadInt(PARAMS(deviceIdx, 2)) != FFFF) {				// Threshold set
		if (abs(previousCommandValue - commandValue) >= (EEPROMReadInt(PARAMS(deviceIdx, 2)))) {
			previousCommandValue = commandValue;
			return true;
		}
	}
	return false;
}

void Device::checkStatus() {
	int previousCommandID = commandID;
	if (previousStatus != status || checkCommandValue()) {
		previousStatus = status;
		commandID = COMMAND_SET_RESULT;
		postMessage(deviceIdx);
	}
	commandID = previousCommandID;
}
