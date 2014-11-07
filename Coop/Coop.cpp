// Do not remove the include below
#include "Coop.h"


Timer timer;
Device mdevices[DEVICE_COUNT];


//The setup function is called once at startup of the sketch
void setup() {
	// Init Web
	setupWeb();

	if (DEBUG_MAIN || DEBUG_DEVICE_HAND || DEBUG_WEB || DEBUG_DEVICE || DEBUG_MEMORY) Serial.begin(57600);
	mdevices[0].begin("Arduino", 98, DEV_TYPE_ARDUINO, 0, (long)60*60*1000, arduinoInit, arduinoCallbackT, arduinoHandler);
	mdevices[1].begin("DHT11", 201, DEV_TYPE_TEMP_HUM, 1, (long)15*60*1000, dht11Init, dht11CallbackT, dht11Handler);
	mdevices[2].begin("Automatic Door", 203, DEV_AUTO_DOOR, 2, (long)0, doorInit, NULL, doorHandler);
//	mdevices[3].begin("Coop Door", DEVICE_3_ID, DEVICE_3_TYPE, DEVICE_3_UPDATE_TIMER, DEVICE_3_INIT, DEVICE_3_CALLBACK, DEVICE_3_COMMANDHANDLER);



	showStatus(INFO_NORMAL);

	arduinoCallback(0);

}

// The loop function is called in an endless loop
void loop() {

	// Is there anything ready for us?
	//if (DEBUG_MEMORY) printMem(" Main ");

	updateWeb();

	// arduinoCallback, Poll Sensors, UpdateLed, StreamData
	timer.update();

}

