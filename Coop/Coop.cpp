// Do not remove the include below
#include "Coop.h"


DEVICE devices[DEVICE_COUNT];
Timer timer;

//template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }


void aliveMessage() ;

long eventTimeout=ALIVE_TIMER;

//The setup function is called once at startup of the sketch
void setup() {
	Serial.begin(57600);

	devices[0].begin(DEVICE_0_ID, DEVICE_0_TYPE);
	devices[1].begin(DEVICE_1_ID, DEVICE_1_TYPE);

	// Init Web
	setupWeb();

	//showStatus(INFO_NORMAL);


//	aliveMessage();

}


// The loop function is called in an endless loop
void loop() {

	// the loop function runs over and over again forever

	timer.update();		// AliveMessage, Poll Sensors, UpdateLed, StreamData

	// Is there anything ready for us?
	updateWeb();

	if((long)millis()-eventTimeout>=0) {
		eventTimeout=(long)millis()+ALIVE_TIMER;
		aliveMessage();
	}
}

void aliveMessage() {
	if (DEBUG_MAIN) Serial.println("*Alive Message");
	devices[0].setCommand(COMMAND_STATUSON);
	postMessage(0, WEB_POST_TYPE_SIMPLE);
}

byte findDevice(uint16_t findwhat) {
	//printf ("Find: '%u'", findwhat);
	for (byte i = 0; i < DEVICE_COUNT; i++) {
		if (findwhat == devices[i].getDeviceid()) return i;
		break;
	}
	return -1;
}

