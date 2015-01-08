/*
 * sensors.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: pvloon
 */
///////////////////////////////////
// includes
#include "Deviceaction.h"

///////////////////////////////////
// init DHT
DHT dht;

void arduinoInit(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("ArdI");
	pinMode(LED_PIN, OUTPUT);
	arduinoCallback(deviceIDidx);
}
// DEVICE_0_CALLBACK
void arduinoCallbackT() {
	arduinoCallback (ARDUINO_IDX);
}

void arduinoCallback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("ArdC");
	arduinoHandler(deviceIDidx, COMMAND_PING, 0);
}

void aHndlrValues(const byte deviceIDidx, const int commandID, const int commandvalue) {
	char a[MAX_EXT_DATA];
	mdevices[deviceIDidx].setCommand(commandID);
	mdevices[deviceIDidx].setStatus(digitalRead(LED_PIN));
	sprintf(a, "{\"M\" : \"%lu\", \"U\" : \"%lu\"}", check_mem(), millis()/1000);
	mdevices[deviceIDidx].setExtData(a);

}

byte arduinoHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem("aHler");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);
	switch (commandID) {
	case COMMAND_ON:
    	digitalWrite(LED_PIN, HIGH);
    	aHndlrValues(deviceIDidx, commandID, commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_OFF:
        digitalWrite(LED_PIN, LOW);
    	aHndlrValues(deviceIDidx, commandID, commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_1:
	    EEPROMWriteInt(deviceIDidx * 6 + 0, commandvalue);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_2:
	    EEPROMWriteInt(deviceIDidx * 6 + 2, commandvalue);
		return HNDLR_OK;
		break;
	case COMMAND_PING:
	case COMMAND_STATUSREQUEST:
    	aHndlrValues(deviceIDidx, commandID, commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	case COMMAND_GET_VALUE:
    	aHndlrValues(deviceIDidx, commandID, commandvalue);
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}

void dhtInit(const byte deviceIDidx) {
	dhtCallback(deviceIDidx);
}

void dhtCallbackT() {
	dhtCallback(DHT_IDX);
}

void dhtCallback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("dhtC");
	dhtHandler(deviceIDidx, COMMAND_PING, 0);
}

byte dhtHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem("dhtH ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	char a[MAX_EXT_DATA];
	switch (commandID) {
	case COMMAND_STATUSREQUEST:
	case COMMAND_GET_VALUE:
	case COMMAND_PING:
		byte chk;
		mdevices[deviceIDidx].setStatus(STATUS_ERROR);
		chk = dht.read(DHT_PIN);
		switch (chk) {
		case DHTLIB_OK:
			//postMessage(deviceIDidx);
			break;
		default:
			mdevices[deviceIDidx].setStatus(STATUS_ERROR);
			mdevices[deviceIDidx].setExtData("");
			postMessage(deviceIDidx);
			return ERROR;
			break;
		}
		mdevices[deviceIDidx].setCommand(commandID);
		mdevices[deviceIDidx].setStatus(STATUS_ON);
		mdevices[deviceIDidx].setValue((int)dht.temperature);
		int temp1;
		int temp2;
		temp1 = (dht.temperature - (int)dht.temperature) * 100;
		temp2 = (dht.humidity - (int)dht.humidity) * 100;
		sprintf(a, "{\"T\" : \"%0d.%d\" , \"H\" : \"%0d.%d\"}", (int)dht.temperature, temp1, (int)dht.humidity, temp2);
		mdevices[deviceIDidx].setExtData(a);
		if (commandID == COMMAND_STATUSREQUEST || commandID == COMMAND_PING) postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	default:
//		mdevices[d].setValueInt(STATUS, STATUS_ERROR);
//		postMessage(d);
		return ERROR;
		break;
	}
	return ERROR;
}

void ntcInit(const byte deviceIDidx) {
	if (DEBUG_MEMORY) printMem("ntcI ");
	ntcCallback(deviceIDidx);
}

void ntcCallbackT() {
	ntcCallback (NTC_0_IDX);
}

void ntcCallback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.print("ntcC ");
	ntcHandler(deviceIDidx, COMMAND_PING, 0);
}

byte ntcHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
//	if (DEBUG_MEMORY) printMem("ntcH ");
//	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
//	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
//	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
//	if (DEBUG_DEVICE_HAND) Serial.print(" Val ");
//	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	int value;

	switch (commandID) {
	case COMMAND_VALUE_1:
		if (deviceIDidx == NTC_0_IDX) {
			EEPROMWriteInt(deviceIDidx * 6 + 0, commandvalue);
			if (DEBUG_DEVICE_HAND) Serial.print("cval: ");
			if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);
		}
		return HNDLR_OK;
		break;
	case COMMAND_STATUSREQUEST:
	case COMMAND_GET_VALUE:
	case COMMAND_PING:
		mdevices[deviceIDidx].setCommand(commandID);
		mdevices[deviceIDidx].setStatus(STATUS_ON);
		value = analogRead(mdevices[deviceIDidx].getPin());
		if (value == 1023 || value < 0) {
			if (DEBUG_DEVICE_HAND) Serial.print("S-ERR");
			if (DEBUG_DEVICE_HAND) Serial.print(mdevices[deviceIDidx].getName());
			if (DEBUG_DEVICE_HAND) Serial.print(" ");
			if (DEBUG_DEVICE_HAND) Serial.println(value);
			showStatus(SENSOR_ERROR, deviceIDidx);
			return ERROR;
		}
		if (deviceIDidx == NTC_0_IDX) {
			value = value + EEPROMReadInt(deviceIDidx * 6 + 0);
//			if (DEBUG_DEVICE_HAND) Serial.println(EEPROMReadInt(NTC_0_ADDRESS));
		}
//		if (DEBUG_DEVICE_HAND) Serial.print("cval: ");
//		if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);
		mdevices[deviceIDidx].setValue(value);
		if (commandID == COMMAND_STATUSREQUEST || commandID == COMMAND_PING) postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}

void relayInit(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.print("*RelI ");
	if (DEBUG_DEVICE_HAND) Serial.println(mdevices[deviceIDidx].getPin());
	pinMode(mdevices[deviceIDidx].getPin(), OUTPUT);
//	pinMode(mdevices[deviceIDidx].getPin(), OUTPUT);
	// relayCallbackT(); Cannot init for each callback shared
}

void relayCallbackT() {
	relayCallback (123);
}

void relayCallback(const byte deviceIDidx) {
	if (DEBUG_MEMORY) printMem("RlyC ");
	relayHandler(RELAY_1_IDX, COMMAND_PING, 0);
	relayHandler(RELAY_2_IDX, COMMAND_PING, 0);
}

byte relayHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem("relH ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	mdevices[deviceIDidx].setStatus(digitalRead(mdevices[deviceIDidx].getPin()));


	switch (commandID) {
	case COMMAND_ON:
    	digitalWrite(mdevices[deviceIDidx].getPin(), HIGH);
		mdevices[deviceIDidx].setCommand(commandID);
		mdevices[deviceIDidx].setStatus(digitalRead(mdevices[deviceIDidx].getPin()));
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_OFF:
    	digitalWrite(mdevices[deviceIDidx].getPin(), LOW);
		mdevices[deviceIDidx].setCommand(commandID);
		mdevices[deviceIDidx].setStatus(digitalRead(mdevices[deviceIDidx].getPin()));
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_STATUSREQUEST:
	case COMMAND_PING:
		mdevices[deviceIDidx].setCommand(commandID);
		mdevices[deviceIDidx].setStatus(digitalRead(mdevices[deviceIDidx].getPin()));
		postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	case COMMAND_GET_VALUE:
		mdevices[deviceIDidx].setCommand(commandID);
		mdevices[deviceIDidx].setStatus(digitalRead(mdevices[deviceIDidx].getPin()));
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}

byte findDeviceIndex(const int _deviceID) {
	byte found = ERROR;
	for (byte i = 0; i < DEVICE_COUNT; i++) {
		if (mdevices[i].getDeviceid() == _deviceID) {
			return i;
		}
	}
	return found;
}


