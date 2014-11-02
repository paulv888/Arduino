// Do not remove the include below
#include "ARDBrigde.h"


DEVICE devices[DEVICE_COUNT];

// How often to send 'hello world to the other unit
const unsigned long interval = 5000; //ms

// When did we last send?
unsigned long last_sent;


static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static uint8_t ip[] = { 192, 168, 2, 126 };


// the setup routine runs once when you press reset:
void setup() {
	Serial.begin(57600);

	//devices[0].begin( DEVICE_0_ID, DEVICE_0_TYPE, DEVICE_0_NODE_ID);
	devices[0].begin(DEVICE_1_ID, DEVICE_1_TYPE, DEVICE_1_NODE_ID);
	//devices[1].begin( DEVICE_2_ID, DEVICE_2_TYPE);

	// initialize the Ethernet adapter
	Ethernet.begin(mac, ip);

	if (DEBUG_MAIN) Serial.print("server is at ");
	if (DEBUG_MAIN) Serial.println(Ethernet.localIP());

	// Init RF
	setupRF();

	// Init Web
	setupWeb();

}

// the loop routine runs over and over again forever:
void loop() {
//	digitalWrite(led, HIGH); // turn the LED on (HIGH is the voltage level)
//	delay(100); // wait for a second
//	digitalWrite(led, LOW); // turn the LED off by making the voltage LOW
//	delay(1000); // wait for a second

	// Is there anything ready for us?
	updateRF();
	updateWeb();
	millisRollover();


	unsigned long now = millis();
	if (now - last_sent >= ALIVE_TIMER) {
		if (DEBUG_MAIN) Serial.print("freeMemory: ");
		if (DEBUG_MAIN) Serial.println(freeMemory());
		//postMessage();
		last_sent = now;
		for (int i = 0; i < DEVICE_COUNT; i++) { // do not query yourself
				/*			command_tRF command;
				 command.command = COMMAND_STATUSREQUEST;
				 sendMessageRF(devices[0].getNodeAddress(), command); */
		}
	}
	delay(10);
}

int findDevice(uint16_t findwhat, int bywhat) {
	//printf ("Find: '%u'", findwhat);
	for (int i = 0; i < DEVICE_COUNT; i++) {
		//printf (" i: %u ", i);
		switch (bywhat) {
		case FIND_BY_NODE_ADDRESS:
			//printf (" node address: '%u'\n", devices[i].getNodeAddress());
			if (findwhat == devices[i].getNodeAddress()) return i;
			break;
		case FIND_BY_DEVICE_ID:
			//printf (" deviceid: '%u'\n", devices[i].getDeviceid());
			if (findwhat == devices[i].getDeviceid()) return i;
			break;
		}
	}
	return -1;
}

