/*
 * Door.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: pvloon
 */
#include "Door.h"

static byte startSwitch;
static byte stopSwitch;
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
 *  Open/close logic
 *	Start direction
 *	Start power
 *	Wait for top/bottom to close
 *	On close stop power (allow for delay stop)
 *	Detect if not moving and Alert
*/

/*
 *
 */
void delayStopDoor (const byte deviceIdx) {

	if (DEBUG_DEVICE_HAND) printMem("=There ");
	timerCheckStuck = timer.stop(timerCheckStuck);
	timerMaxStop = timer.stop(timerMaxStop);
	digitalWrite(POWER_RELAY_PIN, LOW);
	bbreakIsOn = false;
}

void maxRunTime (const byte deviceIdx) {

	if (DEBUG_DEVICE_HAND) printMem("=Max ");
	timerCheckStuck = timer.stop(timerCheckStuck);
	timerMaxStop = timer.stop(timerMaxStop);
	digitalWrite(POWER_RELAY_PIN, LOW);
	bbreakIsOn = false;
	showStatus(DOOR_MAX_RUNTIME, deviceIdx);
	
}

void doorTimer(const byte deviceIdx) {

	//if (DEBUG_DEVICE_HAND) Serial.print(".");

	if( !bcheckStuck && !bbreakIsOn) {
		if (digitalRead(POWER_RELAY_PIN)) {							// We are moving, check if i need to stop
			if (digitalRead(stopSwitch) == LOW) {	 				// Are we there yet!!!
				if (DEBUG_DEVICE_HAND) printMem("=DStop ");
				if (DEBUG_DEVICE_HAND) Serial.println(EEPROMReadInt(PARAMS(deviceIdx, 1)));
				timer.after(EEPROMReadInt(PARAMS(deviceIdx, 1)), delayStopDoor, deviceIdx);
				// TimerCount = 1
				bbreakIsOn = true;
			}

			if (digitalRead(startSwitch) == LOW) {	 					// That is not good
				if (DEBUG_DEVICE_HAND) printMem("=Strange ");
				digitalWrite(POWER_RELAY_PIN, LOW);
				showStatus(DOOR_NOT_MOVING, deviceIdx);
			}
		}
	}										// Monitor status change and post
	mdevices[deviceIdx].readInput();
}

void checkStuck(const byte deviceIdx) {										// Allow time for leave current position
	if (DEBUG_DEVICE_HAND) printMem("=Strt ");

	bcheckStuck = false;
	mdevices[deviceIdx].readInput();
	if (mdevices[deviceIdx].status == STATUS_UNKNOWN) {			// No swithes closed = Is Moving
		if (DEBUG_DEVICE_HAND) Serial.println("=Open");
	} else {
		if (digitalRead(startSwitch) == LOW) {
	    	digitalWrite(POWER_RELAY_PIN, LOW);
			timerMaxStop = timer.stop(timerMaxStop);
			showStatus(DOOR_NOT_MOVING, deviceIdx);
		}
	}

}

void startDoor (const byte deviceIdx, const int commandID) {

	//   bottom LOW  top LOW  -> Not good Open & Close, why stuck switch????
	//   bottom LOW  top HIGH -> Door Closed
	//   bottom HIGH top LOW  -> Door Open
	//   bottom HIGH top HIGH -> Door Moving

	//   startSwitch LOW  stopSwitch LOW  -> Stuck Switch
	//   startSwitch LOW  stopSwitch HIGH -> Start Position
	//   startSwitch HIGH stopSwitch LOW  -> Reached End
	//   startSwitch HIGH stopSwitch HIGH -> Door Moving

	byte doorDirUp = commandID == COMMAND_ON;						// up = true

	timerCheckStuck = timer.stop(timerCheckStuck);
	timerMaxStop = timer.stop(timerMaxStop);

	if (doorDirUp) {
		if (DEBUG_DEVICE_HAND) Serial.println("Up");
		startSwitch = BOTTOM_SWITCH_PIN;
		stopSwitch  = TOP_SWITCH_PIN;
	} else {
		if (DEBUG_DEVICE_HAND) Serial.println("Down");
		startSwitch = TOP_SWITCH_PIN;
		stopSwitch  = BOTTOM_SWITCH_PIN;
	}

	if (DEBUG_DEVICE_HAND) Serial.println(digitalRead(stopSwitch));

	if (digitalRead(stopSwitch) == HIGH) {									// Currently NOT reached

		digitalWrite(DIRECTION_RELAY_PIN, doorDirUp);  						// Direction
		delay (100);

		// If currently closed start reach timer, else give 1 second to start leaving closed
		if (digitalRead(startSwitch) == LOW) {							   // Currently in Starting postion
			bcheckStuck = true;
			if ((timerCheckStuck = timer.after(EEPROMReadInt(PARAMS(deviceIdx, 3)), checkStuck, deviceIdx)) < 0) {	// Starts stuck timer 
			// TimerCount = 2
				if (DEBUG_DEVICE) Serial.print("ETMR");
				showStatus(TIMER_ERROR, deviceIdx);
			}
		}
		if ((timerMaxStop = timer.after(EEPROMReadInt(PARAMS(deviceIdx, 2)), maxRunTime, deviceIdx)) < 0) {		// Start maxRun timer
			// TimerCount = 3
			if (DEBUG_DEVICE) Serial.print("ETMR");
			showStatus(TIMER_ERROR, deviceIdx);
		}
		digitalWrite(POWER_RELAY_PIN, HIGH);
		if (DEBUG_DEVICE_HAND) Serial.println("Starting");
	} else {
		if (DEBUG_DEVICE_HAND) Serial.println("Reached");
		mdevices[deviceIdx].readInput();
	}
}

byte doorOnOff(const byte deviceIdx, const int commandID) {
	startDoor(deviceIdx, commandID);
	return HNDLR_OK;
}



