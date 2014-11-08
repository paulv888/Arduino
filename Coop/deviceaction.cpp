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
Dht11 dht11;


void doorInit(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("DoorI");
	if (DEBUG_DEVICE_HAND) Serial.println(deviceIDidx);

	pinMode(POWER_RELAY, OUTPUT);
	pinMode(DIRECTION_RELAY, OUTPUT);

	pinMode(TOP_SWITCH, INPUT);
	digitalWrite(TOP_SWITCH, HIGH); // connect internal pull-up

	pinMode(BOTTOM_SWITCH, INPUT);
	digitalWrite(BOTTOM_SWITCH, HIGH); // connect internal pull-up

	doorCallbackT();
}

void doorCallbackT() {
	doorCallback (2);
}

void doorCallback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("DoorC");
	doorHandler(deviceIDidx, COMMAND_PING, 0);
}

void arduinoInit(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("ArdI");
	Serial.println(deviceIDidx);
	pinMode(LED_PIN, OUTPUT);
	arduinoCallback(deviceIDidx);
}
// DEVICE_0_CALLBACK
void arduinoCallbackT() {
	arduinoCallback (0);
}

void arduinoCallback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("ArdC");
	arduinoHandler(deviceIDidx, COMMAND_PING, 0);
}

void dht11Init(const byte deviceIDidx) {
	dht11Callback(deviceIDidx);
}

void dht11CallbackT() {
	dht11Callback(1);
}
// DEVICE_1_CALLBACK
void dht11Callback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("dhtC");
	dht11Handler(deviceIDidx, COMMAND_PING, 0);
}

void relayInit(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.print("*RelI ");
	if (DEBUG_DEVICE_HAND) Serial.println(mdevices[deviceIDidx].getPin());
	pinMode(mdevices[deviceIDidx].getPin(), OUTPUT);
	// relayCallbackT(); Cannot init for each callback shared
}

void relayCallbackT() {
	relayCallback (123);
}

void relayCallback(const byte deviceIDidx) {
	relayHandler(RELAY_0_IDX, COMMAND_PING, 0);
	relayHandler(RELAY_1_IDX, COMMAND_PING, 0);
	relayHandler(RELAY_2_IDX, COMMAND_PING, 0);
}

void ntcCallbackT() {
	ntcCallback (ERROR);
}

void ntcCallback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.print("ntcC ");
	relayHandler(NTC_0_IDX, COMMAND_PING, 0);
	relayHandler(NTC_1_IDX, COMMAND_PING, 0);
}

void aHndlrValues(const byte deviceIDidx, const int commandID, const int commandvalue) {
	byte states;
	char a[MAX_EXT_DATA];
	states = digitalRead(LED_PIN);
	if (DEBUG_DEVICE_HAND) Serial.print(" Led ");
	if (DEBUG_DEVICE_HAND) Serial.println(states);
	mdevices[deviceIDidx].setCommand(commandID);
	mdevices[deviceIDidx].setStatus(states);
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

byte dht11Handler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem("dhtH ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	char a[50];
	switch (commandID) {
	case COMMAND_STATUSREQUEST:
	case COMMAND_GET_VALUE:
	case COMMAND_PING:
		byte chk;
		chk = dht11.read(DHT11_PIN);

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
		mdevices[deviceIDidx].setValue((int)dht11.temperature);
		int temp1;
		int temp2;
		temp1 = (dht11.temperature - (int)dht11.temperature) * 100;
		temp2 = (dht11.humidity - (int)dht11.humidity) * 100;
		sprintf(a, "{\"T\" : \"%0d.%d\" , \"H\" : \"%0d.%d\"}", (int)dht11.temperature, temp1, (int)dht11.humidity, temp2);
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

void dHndlrValues(const byte deviceIDidx, const int commandID, const int commandvalue) {
	char a[MAX_EXT_DATA];
	mdevices[deviceIDidx].setCommand(commandID);
	mdevices[deviceIDidx].setStatus(digitalRead(POWER_RELAY));
	sprintf(a, "{\"T\":\"%i\",\"B\":\"%i\",\"P\":\"%i\",\"D\":\"%i\"}", digitalRead(TOP_SWITCH), digitalRead(BOTTOM_SWITCH), digitalRead(POWER_RELAY), digitalRead(DIRECTION_RELAY));
	mdevices[deviceIDidx].setExtData(a);
}

byte doorHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem("doorH ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val: ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	switch (commandID) {
	case COMMAND_ON:
		digitalWrite(POWER_RELAY, HIGH);
		dHndlrValues(deviceIDidx, commandID, commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_OFF:
    	digitalWrite(POWER_RELAY, LOW);
		dHndlrValues(deviceIDidx, commandID, commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_UP:
    	digitalWrite(DIRECTION_RELAY, HIGH);
		dHndlrValues(deviceIDidx, commandID, commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_DOWN:
    	digitalWrite(DIRECTION_RELAY, LOW);
		dHndlrValues(deviceIDidx, commandID, commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_STATUSREQUEST:
	case COMMAND_PING:
		dHndlrValues(deviceIDidx, commandID, commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	case COMMAND_GET_VALUE:
		dHndlrValues(deviceIDidx, commandID, commandvalue);
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}

void doorTimer() {
	if (DEBUG_MEMORY) printMem(" doorTimer ");

	// Here is the main open/close logic
	// Start direction
	// Start power
	// Wait for top/bottom to close
	// On close stop power
	// Detect if not moving and Alert
	if (DEBUG_DEVICE_HAND) Serial.print("*** Door Timer: ");

	byte top = digitalRead(TOP_SWITCH);
	byte bottom = digitalRead(BOTTOM_SWITCH);

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


byte ntcHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem("ntcH ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	int value;

	switch (commandID) {
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
		}
		mdevices[deviceIDidx].setValue(value);
		if (commandID == COMMAND_STATUSREQUEST) postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}
