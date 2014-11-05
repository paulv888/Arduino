// Do not remove the include below
#include "Coop.h"


Timer timer;
Device mdevices[DEVICE_COUNT];


//The setup function is called once at startup of the sketch
void setup() {
	if (DEBUG_MAIN || DEBUG_DEVICE_HAND || DEBUG_WEB || DEBUG_DEVICE || DEBUG_MEMORY) Serial.begin(57600);
	mdevices[0].begin(DEVICE_0_NAME, DEVICE_0_ID, DEVICE_0_TYPE, DEVICE_0_UPDATE_TIMER, DEVICE_0_INIT, DEVICE_0_CALLBACK, DEVICE_0_COMMANDHANDLER);
	mdevices[1].begin(DEVICE_1_NAME, DEVICE_1_ID, DEVICE_1_TYPE, DEVICE_1_UPDATE_TIMER, DEVICE_1_INIT, DEVICE_1_CALLBACK, DEVICE_1_COMMANDHANDLER);
	mdevices[2].begin(DEVICE_2_NAME, DEVICE_2_ID, DEVICE_2_TYPE, DEVICE_2_UPDATE_TIMER, DEVICE_2_INIT, DEVICE_2_CALLBACK, DEVICE_2_COMMANDHANDLER);
	mdevices[3].begin(DEVICE_3_NAME, DEVICE_3_ID, DEVICE_3_TYPE, DEVICE_3_UPDATE_TIMER, DEVICE_3_INIT, DEVICE_3_CALLBACK, DEVICE_3_COMMANDHANDLER);

	// Init Web
	setupWeb();

	showStatus(INFO_NORMAL);

	DEVICE_0_CALLBACK();
	DEVICE_1_CALLBACK();

}

// The loop function is called in an endless loop
void loop() {


	// Is there anything ready for us?
/*	if (DEBUG_MEMORY) {
		Serial.println(freeMemory());
	}
	Serial.print("Main 1 ");
	check_mem();
		  Serial.print(" heapptr ");
		  Serial.print((long)heapptr);
		  Serial.print(" stackptr ");
		  Serial.print((long)stackptr);
		  Serial.println(" free ");
*/
	updateWeb();

	// arduinoCallback, Poll Sensors, UpdateLed, StreamData
	timer.update();

}

