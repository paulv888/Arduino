/*
 * sensors.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: pvloon
 */
///////////////////////////////////
// includes
#include "sensors.h"

///////////////////////////////////
// init DHT
Dht11 dht11;

// DEVICE_0_CALLBACK
void aliveMessage() {
	if (DEBUG_SENSORS) Serial.println("*Alive Message");
	devices[DEVICE_0_IDX].setValueInt(STATUS, STATUS_ON);
	devices[DEVICE_0_IDX].setValueInt(FREEMEMORY, freeMemory());
	devices[DEVICE_0_IDX].setValueUL(UPTIME, millis());
	devices[DEVICE_0_IDX].setValueFloat(TEMPERATURE, GetInternalTemp());
	postMessage(DEVICE_0_IDX);
}
// DEVICE_1_CALLBACK
void getTempHumidity() {
	if (DEBUG_SENSORS) Serial.println("*Get Temperate Humidity");

	int chk = dht11.read(DHT11_PIN);

	switch (chk) {
	case DHTLIB_OK:
		devices[DEVICE_0_IDX].setValueInt(STATUS, STATUS_ON);
		devices[DEVICE_1_IDX].setValueInt(TEMPERATURE, dht11.temperature);
		devices[DEVICE_1_IDX].setValueInt(HUMIDITY,  dht11.humidity);
		postMessage(DEVICE_1_IDX);
		break;
/*	case DHTLIB_ERROR_CHECKSUM:
		break;
	case DHTLIB_ERROR_TIMEOUT:
		break;*/
	default:
		devices[DEVICE_0_IDX].setValueInt(STATUS, STATUS_ERROR);
		devices[DEVICE_1_IDX].setValueInt(TEMPERATURE, 0);
		devices[DEVICE_1_IDX].setValueInt(HUMIDITY,  0);
		postMessage(DEVICE_1_IDX);
		break;
	}
}

void ledOnOff() {
    static boolean led;
	if (DEBUG_SENSORS) Serial.println("*Toggle Led");
    led= !led;
    if (led) {
    	digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
}

void ledOnOff(void *deviceID, void *commandID, void *commandvalue) {
	if (DEBUG_SENSORS) Serial.print("*Exec Command: ");
	//if (DEBUG_SENSORS) Serial.println(*commandID);


}


