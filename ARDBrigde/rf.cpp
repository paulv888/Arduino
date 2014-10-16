// Do not remove the include below
#include "rf.h"

// nRF24L01(+) radio attached using Getting Started board
RF24 radio(RF_CE_PIN, RF_CSN_PIN);

// Network uses that radio
RF24Network network(radio);

// the setup routine runs once when you press reset:
void setupRF() {

	SPI.begin();
	radio.begin();
	network.begin(/*channel*/90, RF_THIS_NODE);
	if (DEBUG_RF) Serial.println("RF24Network/ARDBridge/\n");
	if (DEBUG_RF) Serial.print("RF24Network/this_node: ");
	if (DEBUG_RF) Serial.println(RF_THIS_NODE);
	radio.printDetails();

}

bool updateRF(bool postmessages) {
//
//	Postponed postmessage to end (will loose messages here if non postable comes in after postable, not a queue)
//

	network.update();
	bool read_something = false;
	unsigned long last_received = millis();
	unsigned long now;

	do {
		while (network.available()) {
			last_received = millis();

			RF24NetworkHeader header;
			network.peek(header);
			if (header.type == PAYLOAD_TYPE_VALUES) {
				payLoad_Value_Pair command;
				network.read(header, &command, sizeof(command));
				byte i = findDevice(header.from_node, FIND_BY_NODE_ADDRESS);

				if (DEBUG_RF) Serial.print("<<< ");
				if (DEBUG_RF) Serial.print(header.from_node);
				if (DEBUG_RF) Serial.print(" type ");
				if (DEBUG_RF) Serial.print(header.type);
				if (DEBUG_RF) Serial.print(" command ");
				if (DEBUG_RF) Serial.print(devices[i].getCommand());
				if (DEBUG_RF) Serial.print(" sz ");
				if (DEBUG_RF) Serial.print(sizeof(command));
				if (DEBUG_RF) Serial.print(" sensor ");
				if (DEBUG_RF) Serial.print(command.sensor);
				if (DEBUG_RF) Serial.print(" val ");
				if (DEBUG_RF) Serial.print(command.value);
				if (DEBUG_RF) Serial.print(" age ");
				if (DEBUG_RF) Serial.print(command.age);
				if (DEBUG_RF) Serial.println();

				if (i >= 0) {
					devices[i].setCommand(command.command);
					devices[i].setData(command.data);
					devices[i].setSensorValue(command.sensor, command.value, command.age);
					if (postmessages) devices[i].needpost = WEB_POST_TYPE_VALUES;
				}
				devices[0].rfReceivePackets++;
				read_something = true;
			} else if (header.type == PAYLOAD_TYPE_DEVICE_STATUS) {
				payLoad_DeviceStatus command;
				network.read(header, &command, sizeof(command));
				byte i = findDevice(header.from_node, FIND_BY_NODE_ADDRESS);

				if (DEBUG_RF) Serial.print("<<< ");
				if (DEBUG_RF) Serial.print(header.from_node);
				if (DEBUG_RF) Serial.print(" type ");
				if (DEBUG_RF) Serial.print(header.type);
				if (DEBUG_RF) Serial.print(" command ");
				if (DEBUG_RF) Serial.print(devices[i].getCommand());
				if (DEBUG_RF) Serial.print(" sz ");
				if (DEBUG_RF) Serial.println(sizeof(command));

				if (i >= 0) {
					unpackCommandRF(i, command);
					// do not post these, only received from web query
				}
				devices[0].rfReceivePackets++;
				read_something = true;
			} else if (header.type == PAYLOAD_TYPE_SIMPLE) {
				payLoad_Simple command;
				network.read(header, &command, sizeof(command));
				byte i = findDevice(header.from_node, FIND_BY_NODE_ADDRESS);

				if (DEBUG_RF) Serial.print("<<< ");
				if (DEBUG_RF) Serial.print(header.from_node);
				if (DEBUG_RF) Serial.print(" type ");
				if (DEBUG_RF) Serial.print(header.type);
				if (DEBUG_RF) Serial.print(" command ");
				if (DEBUG_RF) Serial.print(command.command);
				if (DEBUG_RF) Serial.print(" data ");
				if (DEBUG_RF) Serial.print(command.data);
				if (DEBUG_RF) Serial.print(" sz ");
				if (DEBUG_RF) Serial.println(sizeof(command));

				if (i >= 0) {
					devices[i].setCommand(command.command);
					devices[i].setData(command.data);
					if (postmessages) postMessage(i, WEB_POST_TYPE_SIMPLE);
				}
				devices[0].rfReceivePackets++;
				read_something = true;
			}
			network.update();
		}
		network.update();
		now = millis();
	} while (read_something and (now - last_received) < RF_QUIET_WAIT_TIME);

	if (DEBUG_RF) if (read_something) Serial.println("Stop Reading");

	if (postmessages) {
		for (byte i = 0; i < DEVICE_COUNT; i++) {
			if (devices[i].needpost > 0) {
				postMessage(i, devices[i].needpost);
				devices[i].needpost = 0;
			}
		}
	}

	return read_something;
}

void sendMessageRF(byte deviceidx, bool wait_response) {

	if (devices[deviceidx].getNodeAddress() == RF_THIS_NODE) {
		devices[deviceidx].setData(COMMAND_RESULT_OK);
		return;
	}
	payLoad_Simple command;
	command.command = devices[deviceidx].getCommand();
	command.data = devices[deviceidx].getData();
	if (wait_response) devices[deviceidx].clearValues();

	if (DEBUG_RF) Serial.print(">>> ");
	if (DEBUG_RF) Serial.print(devices[deviceidx].getNodeAddress());
	if (DEBUG_RF) Serial.print(" command ");
	if (DEBUG_RF) Serial.print(command.command);
	if (DEBUG_RF) Serial.print(" sz ");
	if (DEBUG_RF) Serial.println(sizeof(command));

	RF24NetworkHeader header(/*to node*/(uint16_t) devices[deviceidx].getNodeAddress());
	bool ok = network.write(header, &command, sizeof(command));
	if (!ok) {
		devices[0].rfSendErrors++;
		devices[deviceidx].setCommand(COMMAND_RESULT_ERROR);
		devices[deviceidx].setData(ERROR_RF_SEND_FAILED);
	} else {
		devices[0].rfTransmitPackets++;
		devices[deviceidx].setData(COMMAND_RESULT_OK);
	}

	unsigned long last_received = millis();
	unsigned long now = millis();
	if (wait_response) {
		bool read_something = false;
		do // wait till timeout or got data
		{
			now = millis();
			read_something = updateRF(true); // should wait for specific response for device// request+answer pair
		} while (((now - last_received) < RF_MAX_WAIT_FOR_RESPONSE) and !read_something);
	}

	if ((now - last_received) >= RF_MAX_WAIT_FOR_RESPONSE) {
		devices[deviceidx].setCommand(COMMAND_RESULT_ERROR);
		devices[deviceidx].setData(ERROR_RF_TIMEOUT);
	}

	return;
}

