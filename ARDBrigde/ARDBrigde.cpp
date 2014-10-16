// Do not remove the include below
#include "ARDBrigde.h"


DEVICE devices[DEVICE_COUNT];

//template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }


static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static uint8_t ip[] = { 192, 168, 2, 126 };
static uint8_t sn[] = { 255, 255, 255, 0 };
static uint8_t gw[] = { 192, 168, 2, 1 };
static uint8_t dns[] = { 192, 168, 2, 1 };


void aliveMessage() ;

long eventTimeout=ALIVE_TIMER;

void setup() {
	Serial.begin(57600);

	devices[0].begin(DEVICE_0_ID, DEVICE_0_TYPE, DEVICE_0_NODE_ID, MAX_SENSORS_0);
	devices[1].begin(DEVICE_1_ID, DEVICE_1_TYPE, DEVICE_1_NODE_ID, MAX_SENSORS_1);
	//devices[1].begin( DEVICE_2_ID, DEVICE_2_TYPE);

	// initialize the Ethernet adapter
	Ethernet.begin(mac, ip, dns, gw, sn );

	if (DEBUG_MAIN) Serial.print("server is at ");
	if (DEBUG_MAIN) Serial.println(Ethernet.localIP());

//	 // fill in the UART file descriptor with pointer to writer.
//	   fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
//
//	   // The uart is the standard output device STDOUT.
//	   stdout = &uartout ;

	// Init RF
	setupRF();

	// Init Web
	setupWeb();

	aliveMessage();

}
//int show1 = 0;

void loop() {

	// Is there anything ready for us?
	updateRF(true);
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

byte findDevice(uint16_t findwhat, byte bywhat) {
	//printf ("Find: '%u'", findwhat);
	for (byte i = 0; i < DEVICE_COUNT; i++) {
		//Serial << " findDevice i: " << i << " \n";
 		switch (bywhat) {
		case FIND_BY_NODE_ADDRESS:
			//Serial << " findDevice Node address: " << devices[i].getNodeAddress() << " \n";
			if (findwhat == devices[i].getNodeAddress()) return i;
			break;
		case FIND_BY_DEVICE_ID:
			//Serial << " findDevice Device ID: " << devices[i].getDeviceid() << " \n";
			if (findwhat == devices[i].getDeviceid()) return i;
			break;
		}
	}
	return -1;
}

