// Do not remove the include below
#include "rf.h"

// nRF24L01(+) radio attached using Getting Started board
RF24 radio(RF_CE_PIN, RF_CSN_PIN);

// Network uses that radio
RF24Network network(radio);

unsigned long rfTransmitPackets = 0;
unsigned long rfReceivePackets = 0;
unsigned long rfSendErrors = 0;

// the setup routine runs once when you press reset:
void setupRF() {

	SPI.begin();
	radio.begin();
	network.begin(/*channel*/90, RF_THIS_NODE);
	if (DEBUG_RF) Serial.println("RF24Network/range/");
	if (DEBUG_RF) Serial.print("RF24Network/this_node: ");
	if (DEBUG_RF) Serial.println(RF_THIS_NODE);
}

// the loop routine runs over and over again forever:
void updateRF() {

	payLoad_Simple command;
	network.update();

	// Is there anything ready for us?
	while (network.available()) {
		// If so, grab it and print it out
		RF24NetworkHeader header;
		network.read(header, &command, sizeof(command));
		if (DEBUG_RF) Serial.print("<<< ");
		if (DEBUG_RF) Serial.print(header.from_node);
		if (DEBUG_RF) Serial.print(" command ");
		if (DEBUG_RF) Serial.print(command.command);
		if (DEBUG_RF) Serial.print(" data ");
		if (DEBUG_RF) Serial.print(command.data);
		if (DEBUG_RF) Serial.print(" sz ");
		if (DEBUG_RF) Serial.println(sizeof(command));

		rfReceivePackets++;
		handle_commandIn(header.from_node, command);
		timer.pulse(LED_PIN, HIGH, 50);
	}
}

bool sendMessageRF(uint16_t sentto, payLoad_Value_Pair &command) {

	RF24NetworkHeader header(/*to node*/sentto);
	header.type = PAYLOAD_TYPE_VALUES;
	timer.pulse(LED_PIN, HIGH, 50);

	int retry = 5;
	bool ok = false;
	do {
		ok = network.write(header, &command, sizeof(command));
		if (!ok) {
			showStatus(WARNING_RF_SEND);
			rfSendErrors++;
			retry--;
			delay (10);
		} else
			rfTransmitPackets++;
	} while (!ok and retry > 0);
	if (DEBUG_RF) Serial.print("payLoad_Range_Values>>> ");
	if (DEBUG_RF) Serial.print(sentto);
	if (DEBUG_RF) Serial.print(" type ");
	if (DEBUG_RF) Serial.print(header.type);
	if (DEBUG_RF) Serial.print(" command ");
	if (DEBUG_RF) Serial.print(command.command);
	if (DEBUG_RF) Serial.print(" sz ");
	if (DEBUG_RF) Serial.print(sizeof(command));
	if (DEBUG_RF) Serial.print(" sensor ");
	if (DEBUG_RF) Serial.print(command.sensor);
	if (DEBUG_RF) Serial.print(" value ");
	if (DEBUG_RF) Serial.print(command.value);
	if (DEBUG_RF) Serial.print(" age ");
	if (DEBUG_RF) Serial.println(command.age);
	return ok;
}

bool sendMessageRF(uint16_t sentto, payLoad_DeviceStatus &command) {

	RF24NetworkHeader header(/*to node*/sentto);
	header.type = PAYLOAD_TYPE_DEVICE_STATUS;

	int retry = 5;
	bool ok = false;
	do {
		ok = network.write(header, &command, sizeof(command));
		if (!ok) {
			showStatus(WARNING_RF_SEND);
			rfSendErrors++;
			retry--;
			delay (10);
		} else
			rfTransmitPackets++;
	} while (!ok and retry > 0);
	if (DEBUG_RF) Serial.print("payLoad_Range_DeviceStatus>>>> ");
	if (DEBUG_RF) Serial.print(sentto);
	if (DEBUG_RF) Serial.print(" type ");
	if (DEBUG_RF) Serial.print(header.type);
	if (DEBUG_RF) Serial.print(" command ");
	if (DEBUG_RF) Serial.print(command.command);
	if (DEBUG_RF) Serial.print(" data ");
	if (DEBUG_RF) Serial.print(command.data);
	if (DEBUG_RF) Serial.print(" sz ");
	if (DEBUG_RF) Serial.println(sizeof(command));
	return ok;
}

bool sendMessageRF(uint16_t sentto, payLoad_Simple &command) {

	RF24NetworkHeader header(/*to node*/sentto);
	header.type = PAYLOAD_TYPE_SIMPLE;

	int retry = 5;
	bool ok = false;
	do {
		ok = network.write(header, &command, sizeof(command));
		if (!ok) {
			showStatus(WARNING_RF_SEND);
			rfSendErrors++;
			retry--;
			delay (100);
		} else
			rfTransmitPackets++;
	} while (!ok and retry > 0);
	if (DEBUG_RF) Serial.print("payLoad_Simple>>> ");
	if (DEBUG_RF) Serial.print(sentto);
	if (DEBUG_RF) Serial.print(" type ");
	if (DEBUG_RF) Serial.print(header.type);
	if (DEBUG_RF) Serial.print(" command ");
	if (DEBUG_RF) Serial.print(command.command);
	if (DEBUG_RF) Serial.print(" data ");
	if (DEBUG_RF) Serial.print(command.data);
	if (DEBUG_RF) Serial.print(" sz ");
	if (DEBUG_RF) Serial.println(sizeof(command));
	return ok;
}
