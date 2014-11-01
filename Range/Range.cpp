#include "Range.h"

void aliveMessage();

Timer timer;

//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
	// initialize serial communications at 9600 bps:
	if (DEBUG_MAIN || DEBUG_RF || DEBUG_SENSORS || DEBUG_COMMAND ) Serial.begin(57600);

	// Use analog ref
	analogReference(EXTERNAL);

	// Setup RF communication
	// setupRF();

	// Init Input switches
	// setupSwitches();

	// Calibrate NTC's
	// calibrate(true);
	// timer.every(SENSOR_CALIBRATION_INTERVAL, calibratetimer);

	showStatus(INFO_NORMAL);

	// timer.every(SENSOR_POL_TIME, updateTempSensors);

	// timer.every(ALIVE_TIMER, aliveMessage);
	// aliveMessage();

}

// The loop function is called in an endless loop
void loop() {

	updateSwitches();
//	updateRF();
	timer.update();		// AliveMessage, Poll Sensors, UpdateLed, StreamData

}

void aliveMessage() {
	if (DEBUG_MAIN) Serial.println("*Alive Message");
	payLoad_Simple command;
	command.command = (currentStatus > 0) ? COMMAND_STATUSON : COMMAND_STATUSOFF;
	command.data = currentStatus;
	showStatus(INFO_NORMAL);
//	sendMessageRF(RF_BASE_STATION, command);
}

