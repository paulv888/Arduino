// Do not remove the include below
#include "Coop.h"


Device devices[DEVICE_COUNT];
Timer timer;

void aliveMessage() ;

int addInt(int n, int m) {
    return n+m;
}

//Passing the pointer to another function is basically the same:

int add2to3(int (*functionPtr)(int, int)) {
    return (*functionPtr)(2, 3);
}

//The setup function is called once at startup of the sketch
void setup() {
	Serial.begin(57600);
	int *commandID;
	int *deviceID;
	int *commandvalue;
	devices[0].begin(DEVICE_0_NAME, DEVICE_0_ID, DEVICE_0_TYPE, DEVICE_0_UPDATE_TIMER, DEVICE_0_CALLBACK, DEVICE_0_COMMANDHANDLER);
	devices[1].begin(DEVICE_1_NAME, DEVICE_1_ID, DEVICE_1_TYPE, DEVICE_1_UPDATE_TIMER, DEVICE_1_CALLBACK, DEVICE_1_COMMANDHANDLER);
//	devices[2].begin(DEVICE_2_NAME, DEVICE_2_ID, DEVICE_2_TYPE, DEVICE_2_UPDATE_TIMER, DEVICE_1_CALLBACK, DEVICE_2_COMMANDHANDLER);

	#define DEVICE_2_COMMANDHANDLER ledOnOff(deviceID, commandID,  commandvalue)




	//First thing, lets define a pointer to a function which receives 2 ints and returns and int:

	int (*functionPtr)(int,int);

	//Now we can safely point to our function:

	functionPtr = &addInt;

	//Now that we have a pointer to the function, lets use it:

	int sum = (*functionPtr)(2, 3); // sum == 5



	ledOnOff(deviceID, commandID,  commandvalue);
	//devices[2].test(ledOnOff(deviceID, commandID,  commandvalue));
	//devices[1].begin(DEVICE_1_ID, DEVICE_1_TYPE, DEVICE_1_CALLBACK);

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



