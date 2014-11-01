// Do not remove the include below
#include "Coop.h"


DEVICE devices[DEVICE_COUNT];
Timer timer;

//template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }


void aliveMessage() ;

//The setup function is called once at startup of the sketch
void setup() {
	Serial.begin(57600);

	devices[0].begin(DEVICE_0_ID, DEVICE_0_TYPE, DEVICE_0_UPDATE_TIMER, DEVICE_0_CALLBACK);
	//devices[1].begin(DEVICE_1_ID, DEVICE_1_TYPE, DEVICE_1_CALLBACK);

	// Init Web
	setupWeb();

	showStatus(INFO_NORMAL);


	// Init Input switches
	// setupSwitches();

	// Calibrate NTC's
	// calibrate(true);
	// timer.every(SENSOR_CALIBRATION_INTERVAL, calibratetimer);


	// timer.every(SENSOR_POL_TIME, updateTempSensors);

}


// The loop function is called in an endless loop
void loop() {
	// the loop function runs over and over again forever

	// Is there anything ready for us?
	updateWeb();

	// AliveMessage, Poll Sensors, UpdateLed, StreamData
	timer.update();

}

// DEVICE_0_CALLBACK
void aliveMessage() {
	if (DEBUG_MAIN) Serial.println("*Alive Message");
	devices[DEVICE_0_IDX].setValue(FREEMEMORY, freeMemory());
	devices[DEVICE_0_IDX].setValue(UPTIME, millis());
	devices[DEVICE_0_IDX].setValueFloat(INTERNALTEMP, GetInternalTemp());
	postMessage(DEVICE_0_IDX);
}



