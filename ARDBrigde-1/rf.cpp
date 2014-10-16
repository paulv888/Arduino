// Do not remove the include below
#include "rf.h"

// nRF24L01(+) radio attached using Getting Started board
RF24 radio(RF_CE_PIN, RF_CSN_PIN);

// Network uses that radio
RF24Network network(radio);

unsigned long rfTransmit = 0;
unsigned long rfReceive = 0;
unsigned long rfSendErrors = 0;
unsigned long rfLastSuccesful = 0;

// the setup routine runs once when you press reset:
void setupRF() {

	SPI.begin();
	radio.begin();
	network.begin(/*channel*/90, RF_THIS_NODE);
	if (DEBUG_RF) Serial.println("RF24Network/ARDBridge/\n");
	if (DEBUG_RF) Serial.print("RF24Network/this_node: ");
	if (DEBUG_RF) Serial.println(RF_THIS_NODE);
}

// the loop routine runs over and over again forever:
void updateRF() {

	command_tRF command;
	network.update();

	// Is there anything ready for us?
	while (network.available()) {
		// If so, grab it and print it out
		RF24NetworkHeader header;
		network.read(header, &command, sizeof(command));
		rfLastSuccesful = millis();
		if (DEBUG_RF)	Serial.print("Message from ");
		if (DEBUG_RF)	Serial.print(header.from_node);
		if (DEBUG_RF) Serial.print(" command ");
		if (DEBUG_RF) Serial.print(command.command);
		if (DEBUG_RF) Serial.print(" sz ");
		if (DEBUG_RF) Serial.println(sizeof(command));
		rfReceive++;
		handle_commandRF(header.from_node, command);
	}
}

void sendMessageRF(uint16_t sentto, command_tRF command) {
	// If it's time to send a message, send it!

	if (DEBUG_RF) Serial.print("Sending to: ");
	if (DEBUG_RF) Serial.print( sentto);
	if (DEBUG_RF) Serial.print(" command ");
	if (DEBUG_RF) Serial.print(command.command);
	if (DEBUG_RF) Serial.print(" sz ");
	if (DEBUG_RF) Serial.println( sizeof(command));
	RF24NetworkHeader header(/*to node*/(uint16_t) sentto);
	bool ok = network.write(header, &command, sizeof(command));
	rfTransmit++;
	if (!ok)
		rfSendErrors++;
	else
		rfLastSuccesful = millis();
	if (DEBUG_RF) if (ok) {
			Serial.print("Send  ok.\n");
	}
	if (DEBUG_RF) if (!ok) {
			Serial.print(" Send failed.\n");
	}
}

