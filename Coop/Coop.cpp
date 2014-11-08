// Do not remove the include below
#include "Coop.h"


Timer timer;
Device mdevices[DEVICE_COUNT];


//The setup function is called once at startup of the sketch
void setup() {
	// Init Web
	setupWeb();

	if (DEBUG_MAIN || DEBUG_DEVICE_HAND || DEBUG_WEB || DEBUG_DEVICE || DEBUG_MEMORY) Serial.begin(57600);
	mdevices[0].begin("Arduino", 98, 0, (long)60*60*1000, arduinoInit, arduinoCallbackT, arduinoHandler);
	mdevices[1].begin("DHT11", 201, 1, (long)15*60*1000, dht11Init, dht11CallbackT, dht11Handler);
	mdevices[2].begin("Door", 203, 2, (long)(long)12*60*60*1000, doorInit, doorCallbackT, doorHandler);
	mdevices[RELAY_0_IDX].setPin(A3);
	mdevices[RELAY_0_IDX].begin("Fan", 204, RELAY_0_IDX, (long)0, relayInit, NULL, relayHandler);		// Special Timer taking care off all simple devices
	mdevices[RELAY_1_IDX].setPin(A4);
	mdevices[RELAY_1_IDX].begin("Red Light", 205, RELAY_1_IDX, (long)0, relayInit, NULL, relayHandler);
	mdevices[RELAY_2_IDX].setPin(A5);
	mdevices[RELAY_2_IDX].begin("Coop Light", 206, RELAY_2_IDX, (long)12*60*60*1000, relayInit, relayCallbackT, relayHandler);
	//mdevices[6].begin("Heater", 207, 6, (long)12*60*60*1000, relayInit, relayCallbackT, relayHandler);
	mdevices[NTC_0_IDX].setPin(A0);
	mdevices[NTC_0_IDX].begin("NTC 1", 208, NTC_0_IDX, (long)0, NULL, NULL, ntcHandler);
	mdevices[NTC_1_IDX].setPin(A1);
	mdevices[NTC_1_IDX].begin("NTC 2", 209, NTC_1_IDX, (long)0, NULL, NULL, ntcHandler);
	relayCallbackT();

	showStatus(INFO_NORMAL, 0);

	// Use analog ref
	//analogReference(EXTERNAL);

}

// The loop function is called in an endless loop
void loop() {

	//if (DEBUG_MEMORY) printMem(" Main ");

	updateWeb();

	timer.update();

}

