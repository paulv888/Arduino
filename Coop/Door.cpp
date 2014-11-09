/*
 * Door.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */
#include "Door.h"

static byte startSwitch;
static byte stopSwitch;
static byte lastDoorPosition = -1;
static bool bcheckStuck = false;
static bool bbreakIsOn = false;
static int timerCheckStuck = -1;
static int timerMaxStop = -1;


/* Test Scenario's
 * 		Door open on command on -> do nothing
 * 		Door closed on command off -> do nothing
 * 		Door closed on command on -> start opening
 * 		Door open on command off -> start closing
 * 		On opening and reach top -> stop
 * 		On closing and reach botton -> stop
 * 		On opening and still at bottom after 1 sec -> Stop send error
 * 		On closing and still at top after 1 sec -> Stop send error
 *
 */

/*
 *
 */
void dHndlrValues(const byte deviceIDidx, const int commandID, const int status, const int commandvalue) {
	char a[MAX_EXT_DATA];
	mdevices[deviceIDidx].setCommand(commandID);
	mdevices[deviceIDidx].setStatus(status);
	sprintf(a, "{\"T\":\"%i\",\"B\":\"%i\",\"P\":\"%i\",\"D\":\"%i\"}", digitalRead(TOP_SWITCH), digitalRead(BOTTOM_SWITCH), digitalRead(POWER_RELAY), digitalRead(DIRECTION_RELAY));
	mdevices[deviceIDidx].setExtData(a);
}

void doorInit(const byte deviceIDidx) {

	if (DEBUG_DEVICE_HAND) Serial.println("DoorI");
	if (DEBUG_DEVICE_HAND) Serial.println(deviceIDidx);

	pinMode(POWER_RELAY, OUTPUT);
	pinMode(DIRECTION_RELAY, OUTPUT);

	pinMode(TOP_SWITCH, INPUT);
	digitalWrite(TOP_SWITCH, HIGH); // connect internal pull-up

	pinMode(BOTTOM_SWITCH, INPUT);
	digitalWrite(BOTTOM_SWITCH, HIGH); // connect internal pull-up

}

void doorCallbackT() {
	doorCallback (DOOR_IDX);
}

int doorPosition () {
	if (!digitalRead(TOP_SWITCH)) {
		return STATUS_ON;
	} else if (!digitalRead(BOTTOM_SWITCH)) {
		return STATUS_OFF;
	} else {
		return STATUS_UNKNOWN;
	}
}

void delayStopDoor () {

	if (DEBUG_DEVICE_HAND) printMem("=There ");
	timerCheckStuck = timer.stop(timerCheckStuck);
	timerMaxStop = timer.stop(timerMaxStop);
	digitalWrite(POWER_RELAY, LOW);
	bbreakIsOn = false;
}

void hardStopDoor () {

	if (DEBUG_DEVICE_HAND) printMem("=Max ");
	timerCheckStuck = timer.stop(timerCheckStuck);
	timerMaxStop = timer.stop(timerMaxStop);
	digitalWrite(POWER_RELAY, LOW);
	bbreakIsOn = false;
}

void doorCallback(const byte deviceIDidx) {

//	if (DEBUG_DEVICE_HAND) Serial.println("Checking ");

	if( !bcheckStuck && !bbreakIsOn) {
		if (digitalRead(POWER_RELAY)) {									// We are moving, check if i need to stop
			if (digitalRead(stopSwitch) == LOW) {	 					// Are we there yet!!!
				if (DEBUG_DEVICE_HAND) printMem("=DStop ");
				if (DEBUG_DEVICE_HAND) Serial.println(EEPROMReadInt(DOOR_DELAY_ADDRESS));
				timer.after(EEPROMReadInt(DOOR_DELAY_ADDRESS), delayStopDoor);
				bbreakIsOn = true;
			}

			if (digitalRead(startSwitch) == LOW) {	 					// That is not good
				if (DEBUG_DEVICE_HAND) printMem("=Strange ");
				digitalWrite(POWER_RELAY, LOW);
				showStatus(DOOR_NOT_MOVING, DOOR_IDX);
			}
		}
	}															// Monitor status change and post

	if (lastDoorPosition != doorPosition()) {
		lastDoorPosition = doorPosition();

		if (DEBUG_DEVICE_HAND) Serial.print("Change ");
		if (DEBUG_DEVICE_HAND) Serial.println(lastDoorPosition);
		dHndlrValues(DOOR_IDX, COMMAND_SET_RESULT, lastDoorPosition, 0);
		postMessage(DOOR_IDX);
		showStatus(INFO_NORMAL, DOOR_IDX);
	}
}

void checkStuck() {										// Allow time for leave current position
	if (DEBUG_DEVICE_HAND) printMem("=Strt ");

	// Here is the main open/close logic
	// Start direction
	// Start power
	// Wait for top/bottom to close
	// On close stop power
	// Detect if not moving and Alert

	bcheckStuck = false;
	if (doorPosition() == STATUS_UNKNOWN) {			// No swithes closed = Is Moving
		if (DEBUG_DEVICE_HAND) Serial.println("=Open");
	} else {
		if (digitalRead(startSwitch) == LOW) {
	    	digitalWrite(POWER_RELAY, LOW);
			showStatus(DOOR_NOT_MOVING, DOOR_IDX);
		}
	}

}

void startDoor (int commandID) {

	byte doorDirUp = commandID == COMMAND_ON;						// up = true

	timerCheckStuck = timer.stop(timerCheckStuck);
	timerMaxStop = timer.stop(timerMaxStop);

	if (doorDirUp) {
		if (DEBUG_DEVICE_HAND) Serial.println("Up");
		startSwitch = BOTTOM_SWITCH;
		stopSwitch = TOP_SWITCH;
	} else {
		if (DEBUG_DEVICE_HAND) Serial.println("Down");
		startSwitch = TOP_SWITCH;
		stopSwitch = BOTTOM_SWITCH;
	}

	if (DEBUG_DEVICE_HAND) Serial.println(digitalRead(stopSwitch));

	if (digitalRead(stopSwitch) != LOW) {									// Currently NOT reached

		digitalWrite(DIRECTION_RELAY, doorDirUp);  						// Direction
		delay (100);

		// If currently closed start reach timer, else give 1 second to start leaving closed
		if (digitalRead(startSwitch) == LOW) {							   // Currently in Starting postion
			bcheckStuck = true;
			if ((timerCheckStuck = timer.after(3000, checkStuck)) < 0) {						// Starts moving timer as well (time /2 clockspeed
				if (DEBUG_DEVICE) Serial.print("ETMR");
				showStatus(TIMER_ERROR, DOOR_IDX);
			}
		}
		if ((timerMaxStop = timer.after(EEPROMReadInt(DOOR_MAX_RUNTIME_ADDRESS), hardStopDoor)) < 0) {
			if (DEBUG_DEVICE) Serial.print("ETMR");
			showStatus(TIMER_ERROR, DOOR_IDX);
		}
		digitalWrite(POWER_RELAY, HIGH);
		if (DEBUG_DEVICE_HAND) Serial.println("Starting");
		dHndlrValues(DOOR_IDX, commandID, STATUS_UNKNOWN, 0);
	} else {
		if (DEBUG_DEVICE_HAND) Serial.println("=InStart");
		dHndlrValues(DOOR_IDX, commandID, doorPosition(), 0);
	}
}

void postMessageDoor(){
/*
 * 	No guarantees on state device after 100ms
 */
	postMessage(DOOR_IDX);
}

byte doorHandler(const byte deviceIDidx, const int commandID, const int commandvalue) {
	if (DEBUG_MEMORY) printMem("doorH ");
	if (DEBUG_DEVICE_HAND) Serial.print(deviceIDidx);
	if (DEBUG_DEVICE_HAND) Serial.print(" Cmd ");
	if (DEBUG_DEVICE_HAND) Serial.print(commandID);
	if (DEBUG_DEVICE_HAND) Serial.print(" Val: ");
	if (DEBUG_DEVICE_HAND) Serial.println(commandvalue);

	switch (commandID) {
	case COMMAND_ON:													// Open
		startDoor(commandID);
		timer.after(10,postMessageDoor);
		return HNDLR_OK;
		break;
	case COMMAND_OFF:												// Closed
		startDoor(commandID);
		timer.after(10,postMessageDoor);
		return HNDLR_OK;
		break;
	case COMMAND_STOP:
    	digitalWrite(POWER_RELAY, LOW);
		dHndlrValues(deviceIDidx, commandID, doorPosition(), commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_UP:
    	digitalWrite(DIRECTION_RELAY, HIGH);
		dHndlrValues(deviceIDidx, commandID, doorPosition(), commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_DOWN:
    	digitalWrite(DIRECTION_RELAY, LOW);
		dHndlrValues(deviceIDidx, commandID, doorPosition(), commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_1:
	     EEPROMWriteInt(DOOR_DELAY_ADDRESS, commandvalue);
		return HNDLR_OK;
		break;
	case COMMAND_VALUE_2:
	     EEPROMWriteInt(DOOR_MAX_RUNTIME_ADDRESS, commandvalue);
		return HNDLR_OK;
		break;
	case COMMAND_STATUSREQUEST:
	case COMMAND_SET_RESULT:
	case COMMAND_PING:
		dHndlrValues(deviceIDidx, commandID, doorPosition(), commandvalue);
		postMessage(deviceIDidx);
		return HNDLR_WRITE_RESULT;
		break;
	case COMMAND_GET_VALUE:
		dHndlrValues(deviceIDidx, commandID, doorPosition(), commandvalue);
		return HNDLR_WRITE_RESULT;
		break;
	default:
		return ERROR;
		break;
	}
	return ERROR;
}

