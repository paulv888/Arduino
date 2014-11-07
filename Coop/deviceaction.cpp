/*
 * sensors.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: pvloon
 */
///////////////////////////////////
// includes
#include "deviceaction.h"

///////////////////////////////////
// init DHT
Dht11 dht11;


void doorInit(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("Door Init");
	Serial.print("Init _Idx ");
	Serial.println(deviceIDidx);

	pinMode(POWER_RELAY, OUTPUT);
	pinMode(DIRECTION_RELAY, OUTPUT);

	pinMode(TOP_SWITCH, INPUT);
	digitalWrite(TOP_SWITCH, HIGH); // connect internal pull-up

	pinMode(BOTTOM_SWITCH, INPUT);
	digitalWrite(BOTTOM_SWITCH, HIGH); // connect internal pull-up
}

void arduinoInit(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("Arduino Init");
	pinMode(LED_PIN, OUTPUT);
	//arduinoCallback(deviceIDidx);
}
// DEVICE_0_CALLBACK
void arduinoCallbackT() {
	arduinoCallback (0);
}

void arduinoCallback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("*Alive Message");
	arduinoHandler(deviceIDidx, COMMAND_GET_VALUE, 0);

	Serial.println("*returned aHand");

	postMessage(deviceIDidx);
}

void dht11Init(const byte deviceIDidx) {
	dht11Callback(deviceIDidx);
}

void dht11CallbackT() {
	dht11Callback(1);
}
// DEVICE_1_CALLBACK
void dht11Callback(const byte deviceIDidx) {
	if (DEBUG_DEVICE_HAND) Serial.println("*Get Temperate Humidity");
	dht11Handler(deviceIDidx, COMMAND_GET_VALUE, 0);
}

byte arduinoHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem(" arduinoHandler ");
	if (DEBUG_DEVICE_HAND) Serial.print("*Exec deviceIdx: ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" command: ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" value: ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);
	byte states;
	char a[MAX_EXT_DATA];
	switch (commandID) {
	case COMMAND_ON:
    	digitalWrite(LED_PIN, HIGH);
  		states = digitalRead(LED_PIN);
		if (DEBUG_DEVICE_HAND) Serial.print(" LED STATE: ");
		if (DEBUG_DEVICE_HAND) Serial.println(states);
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(states);
		mdevices[deviceIDidx].setInOut(COMMAND_IO_RECV);
		sprintf(a, "{\"M\" : \"%lu\", \"U\" : \"%lu\"}", check_mem(), millis()/1000);
		mdevices[deviceIDidx].setExtData(a);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_OFF:
        digitalWrite(LED_PIN, LOW);
  		states = digitalRead(LED_PIN);
		if (DEBUG_DEVICE_HAND) Serial.print(" LED STATE: ");
		if (DEBUG_DEVICE_HAND) Serial.println(states);
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(states);
		mdevices[deviceIDidx].setInOut(COMMAND_IO_RECV);
		sprintf(a, "{\"M\" : \"%lu\", \"U\" : \"%lu\"}", check_mem(), millis()/1000);
		mdevices[deviceIDidx].setExtData(a);
		if (DEBUG_DEVICE_HAND) Serial.print(" ExtData: ");
		if (DEBUG_DEVICE_HAND) Serial.println("");
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_STATUSREQUEST:
	case COMMAND_GET_VALUE:
  		states = digitalRead(LED_PIN);
		if (DEBUG_DEVICE_HAND) Serial.print(" LED STATE: ");
		if (DEBUG_DEVICE_HAND) Serial.println(states);
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(states);
		mdevices[deviceIDidx].setInOut(COMMAND_IO_RECV);
		sprintf(a, "{\"M\" : \"%lu\", \"U\" : \"%lu\"}", check_mem(), millis()/1000);
		if (DEBUG_DEVICE_HAND) Serial.print(" millis: ");
		if (DEBUG_DEVICE_HAND) Serial.println(millis());
		mdevices[deviceIDidx].setExtData(a);
		if (commandID == COMMAND_STATUSREQUEST) postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}

byte dht11Handler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem(" dht11Handler ");
	if (DEBUG_DEVICE_HAND) Serial.print("*Exec deviceIdx: ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" command: ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" value: ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	char a[50];
	switch (commandID) {
	case COMMAND_STATUSREQUEST:
	case COMMAND_GET_VALUE:
		byte chk;
		chk = dht11.read(DHT11_PIN);

		switch (chk) {
		case DHTLIB_OK:
			//postMessage(deviceIDidx);
			break;
		default:
			mdevices[deviceIDidx].setStatus(STATUS_ERROR);
			mdevices[deviceIDidx].setExtData("");
			mdevices[deviceIDidx].setInOut(COMMAND_IO_RECV);
			postMessage(deviceIDidx);
			return ERROR;
			break;
		}
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(STATUS_ON);
		mdevices[deviceIDidx].setInOut(COMMAND_IO_RECV);
		int temp1;
		int temp2;
		temp1 = (dht11.temperature - (int)dht11.temperature) * 100;
		temp2 = (dht11.humidity - (int)dht11.humidity) * 100;
		sprintf(a, "{\"T\" : \"%0d.%d\" , \"H\" : \"%0d.%d\"}", (int)dht11.temperature, temp1, (int)dht11.humidity, temp2);
		mdevices[deviceIDidx].setExtData(a);
		if (commandID == COMMAND_STATUSREQUEST) postMessage(deviceIDidx);
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

byte doorHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem(" doorHandler ");
	if (DEBUG_DEVICE_HAND) Serial.print("*Exec deviceIdx: ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" command: ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" value: ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);
	char a[50];
	switch (commandID) {
	case COMMAND_ON:
    	digitalWrite(POWER_RELAY, HIGH);
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(digitalRead(POWER_RELAY));
		mdevices[deviceIDidx].setInOut(COMMAND_IO_RECV);
		sprintf(a, "{\"T\":\"%i\",\"B\":\"%i\",\"P\":\"%i\",\"D\":\"%i\"}", digitalRead(TOP_SWITCH), digitalRead(BOTTOM_SWITCH), digitalRead(POWER_RELAY), digitalRead(DIRECTION_RELAY));
		Serial.print("####");
		Serial.println(a);
		mdevices[deviceIDidx].setExtData(a);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_OFF:
    	digitalWrite(POWER_RELAY, LOW);
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(digitalRead(POWER_RELAY));
		mdevices[deviceIDidx].setInOut(COMMAND_IO_RECV);
		sprintf(a, "{\"T\":\"%i\",\"B\":\"%i\",\"P\":\"%i\",\"D\":\"%i\"}", digitalRead(TOP_SWITCH), digitalRead(BOTTOM_SWITCH), digitalRead(POWER_RELAY), digitalRead(DIRECTION_RELAY));
		mdevices[deviceIDidx].setExtData(a);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_UP:
    	digitalWrite(DIRECTION_RELAY, HIGH);
		if (DEBUG_DEVICE_HAND) Serial.print("Sw: ");
		if (DEBUG_DEVICE_HAND) Serial.println(a);
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(digitalRead(POWER_RELAY));
		mdevices[deviceIDidx].setInOut(COMMAND_IO_RECV);
		sprintf(a, "{\"T\":\"%i\",\"B\":\"%i\",\"P\":\"%i\",\"D\":\"%i\"}", digitalRead(TOP_SWITCH), digitalRead(BOTTOM_SWITCH), digitalRead(POWER_RELAY), digitalRead(DIRECTION_RELAY));
		mdevices[deviceIDidx].setExtData(a);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_DOWN:
    	digitalWrite(DIRECTION_RELAY, LOW);
		if (DEBUG_DEVICE_HAND) Serial.print("Sw: ");
		if (DEBUG_DEVICE_HAND) Serial.println(a);
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(digitalRead(POWER_RELAY));
		mdevices[deviceIDidx].setInOut(COMMAND_IO_RECV);
		sprintf(a, "{\"T\":\"%i\",\"B\":\"%i\",\"P\":\"%i\",\"D\":\"%i\"}", digitalRead(TOP_SWITCH), digitalRead(BOTTOM_SWITCH), digitalRead(POWER_RELAY), digitalRead(DIRECTION_RELAY));
		mdevices[deviceIDidx].setExtData(a);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_STATUSREQUEST:
	case COMMAND_GET_VALUE:
		if (DEBUG_DEVICE_HAND) Serial.print("Sw: ");
		if (DEBUG_DEVICE_HAND) Serial.println(a);
		mdevices[deviceIDidx].setCommand(COMMAND_SET_RESULT);
		mdevices[deviceIDidx].setStatus(digitalRead(POWER_RELAY));
		mdevices[deviceIDidx].setInOut(COMMAND_IO_RECV);
		sprintf(a, "{\"T\":\"%i\",\"B\":\"%i\",\"P\":\"%i\",\"D\":\"%i\"}", digitalRead(TOP_SWITCH), digitalRead(BOTTOM_SWITCH), digitalRead(POWER_RELAY), digitalRead(DIRECTION_RELAY));
		mdevices[deviceIDidx].setExtData(a);
		if (commandID == COMMAND_STATUSREQUEST) postMessage(deviceIDidx);
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

byte findDeviceIndex(const int _deviceID) {
	byte found = ERROR;
	for (byte i = 0; i < DEVICE_COUNT; i++) {
		if (mdevices[i].getDeviceid() == _deviceID) {
			return i;
		}
	}
	return found;
}

