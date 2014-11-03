// Do not remove the include below
#include "Coop.h"


Device devices[DEVICE_COUNT];
Timer timer;

void aliveMessage() ;


//The setup function is called once at startup of the sketch
void setup() {
	Serial.begin(57600);
	devices[0].begin(DEVICE_0_NAME, DEVICE_0_ID, DEVICE_0_TYPE, DEVICE_0_UPDATE_TIMER, DEVICE_0_CALLBACK, DEVICE_0_COMMANDHANDLER);
	devices[1].begin(DEVICE_1_NAME, DEVICE_1_ID, DEVICE_1_TYPE, DEVICE_1_UPDATE_TIMER, DEVICE_1_CALLBACK, DEVICE_1_COMMANDHANDLER);
	devices[2].begin(DEVICE_2_NAME, DEVICE_2_ID, DEVICE_2_TYPE, DEVICE_2_UPDATE_TIMER, DEVICE_1_CALLBACK, DEVICE_2_COMMANDHANDLER);


	// Init Web
	setupWeb();

	showStatus(INFO_NORMAL);

	DEVICE_0_CALLBACK();
	DEVICE_1_CALLBACK();
}


// The loop function is called in an endless loop
void loop() {

	// Is there anything ready for us?
	updateWeb();

	// AliveMessage, Poll Sensors, UpdateLed, StreamData
	timer.update();

}

