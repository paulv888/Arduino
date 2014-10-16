/*
 * Web.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "Web.h"

//static uint8_t vlosite[] = { 192, 168, 2, 110 };
static uint8_t vlosite[] = { 192, 168, 2, 101 };

EthernetClient myclient;
// ROM-based messages used by the application
// These are needed to avoid having the strings use up our limited
//    amount of RAM.
P(Page_start) = "<html><head><title>ARD_Bridge</title></head><body>\n";
P(Page_end) = "</body></html>";
P(OpenTag) = "<";
P(CloseTag) = "</";
P(Tail_end) = "\n";
P(DeviceID) = "DID>";
P(CommandID) = "CID>";
P(sSensors) = "SNS>";
P(sValue) = "V>";
P(sAge) = "AGE>";
P(Uptime) = "UPT>";
P(sFreeMemory) = "MEM>";
P(sInternalTemp) = "ITP>";
P(sRFPacketsSent) = "RFS>";
P(sRFPacketsReceived) = "RFR>";
P(sRFSendErrors) = "RFE>";
P(sResponse) = "RSP>";
P(sResult) = "RSL>";

int printP(const prog_char *str, bool getLen = false) {
	// copy data out of program memory into local storage, write out in
	// chunks of 32 bytes to avoid extra short TCP/IP packets

	if (!getLen) {

		uint8_t buffer[32];
		size_t bufferEnd = 0;
		while (buffer[bufferEnd++] = pgm_read_byte(str++)) {
			if (bufferEnd == 32) {
				myclient.write(buffer, 32);
				bufferEnd = 0;
			}
		}

		// write out everything left but trailing NUL
		if (bufferEnd > 1) myclient.write(buffer, bufferEnd - 1);
	}
	return strlen_P(str);
}

int printV(char * variable, bool getLen = false) {
	if (!getLen) myclient.print(variable);
	return strlen(variable);
}

int printV(int variable, bool getLen = false) {
	if (!getLen) myclient.print(variable);
	char buffer[16];
	itoa(variable, buffer, 10);
	int a = strlen(buffer);
	return a;
}

/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
#define PREFIX ""
WebServer webserver(PREFIX, 80);

int printSensorValues(EthernetClient client, byte deviceidx, bool getLen = false) {

	myclient = client;
	int len = 0;
	len += printP(OpenTag, getLen);
	len += printP(sResponse, getLen);
		len += printP(OpenTag, getLen);
		len += printP(DeviceID, getLen);
		len += printV(devices[deviceidx].getDeviceid(), getLen);
		len += printP(CloseTag, getLen);
		len += printP(DeviceID, getLen);
		len += printP(Tail_end, getLen);

		len += printP(OpenTag, getLen);
		len += printP(CommandID, getLen);
		len += printV(devices[deviceidx].getCommand(), getLen);
		len += printP(CloseTag, getLen);
		len += printP(CommandID, getLen);
		len += printP(Tail_end, getLen);

		len += printP(OpenTag, getLen);
		len += printP(sResult, getLen);
		len += printV(devices[deviceidx].getData(), getLen);
		len += printP(CloseTag, getLen);
		len += printP(sResult, getLen);
		len += printP(Tail_end, getLen);

		len += printP(OpenTag, getLen);
		len += printP(sSensors, getLen);
		len += printP(Tail_end, getLen);

			for (byte sensorid = 0; sensorid < devices[deviceidx].getMaxSensors(); sensorid++) {
				len += printP(OpenTag, getLen);
				len += printP(sValue, getLen);
				len += printV(devices[deviceidx].getSensorValue(sensorid), getLen);
				len += printP(CloseTag, getLen);
				len += printP(sValue, getLen);
				len += printP(Tail_end, getLen);

				len += printP(OpenTag, getLen);
				len += printP(sAge, getLen);
				len += printV(devices[deviceidx].getSensorAge(sensorid), getLen);
				len += printP(CloseTag, getLen);
				len += printP(sAge, getLen);
				len += printP(Tail_end, getLen);
			}

		len += printP(CloseTag, getLen);
		len += printP(sSensors, getLen);
		len += printP(Tail_end, getLen);

	len += printP(CloseTag, getLen);
	len += printP(sResponse, getLen);
	return len;
}

void printDeviceStatus(WebServer &server, byte deviceidx) {

	server.printP(OpenTag);
	server.printP(sResponse);
		server.printP(OpenTag);
		server.printP(DeviceID);
		server.print(devices[deviceidx].getDeviceid());
		server.printP(CloseTag);
		server.printP(DeviceID);
		server.printP(Tail_end);

		server.printP(OpenTag);
		server.printP(CommandID);
		server.print(devices[deviceidx].getCommand());
		server.printP(CloseTag);
		server.printP(CommandID);
		server.printP(Tail_end);

		server.printP(OpenTag);
		server.printP(Uptime);
		server.print(devices[deviceidx].getUptime());
		server.printP(CloseTag);
		server.printP(Uptime);
		server.printP(Tail_end);

		server.printP(OpenTag);
		server.printP(sFreeMemory);
		server.print(devices[deviceidx].getFreeMemory());
		server.printP(CloseTag);
		server.printP(sFreeMemory);
		server.printP(Tail_end);

		server.printP(OpenTag);
		server.printP(sRFPacketsSent);
		server.print((int) (devices[deviceidx].rfTransmitPackets));
		server.printP(CloseTag);
		server.printP(sRFPacketsSent);
		server.printP(Tail_end);

		server.printP(OpenTag);
		server.printP(sRFPacketsReceived);
		server.print((int) (devices[deviceidx].rfReceivePackets));
		server.printP(CloseTag);
		server.printP(sRFPacketsReceived);
		server.printP(Tail_end);

		server.printP(OpenTag);
		server.printP(sRFSendErrors);
		server.print(devices[deviceidx].rfSendErrors);
		server.printP(CloseTag);
		server.printP(sRFSendErrors);
		server.printP(Tail_end);

		server.printP(OpenTag);
		server.printP(sInternalTemp);
		server.print(devices[deviceidx].getInternalTemp());
		server.printP(CloseTag);
		server.printP(sInternalTemp);
		server.printP(Tail_end);

	server.printP(CloseTag);
	server.printP(sResponse);
}

int printSimpleResponse(EthernetClient client, byte deviceidx, bool getLen = false) {

	myclient = client;

	int len = 0;
	len += printP(OpenTag, getLen);
	len += printP(sResponse, getLen);

		len += printP(OpenTag, getLen);
		len += printP(DeviceID, getLen);
		len += printV(devices[deviceidx].getDeviceid(), getLen);
		len += printP(CloseTag, getLen);
		len += printP(DeviceID, getLen);
		len += printP(Tail_end, getLen);

		len += printP(OpenTag, getLen);
		len += printP(CommandID, getLen);
		len += printV(devices[deviceidx].getCommand(), getLen);
		len += printP(CloseTag, getLen);
		len += printP(CommandID, getLen);
		len += printP(Tail_end, getLen);

		len += printP(OpenTag, getLen);
		len += printP(sResult, getLen);
		len += printV(devices[deviceidx].getData(), getLen);
		len += printP(CloseTag, getLen);
		len += printP(sResult, getLen);
		len += printP(Tail_end, getLen);

	len += printP(CloseTag, getLen);
	len += printP(sResponse, getLen);
	return len;
}

#define NAMELEN 32
#define VALUELEN 32

void processCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
	URLPARAM_RESULT rc;
	char name[NAMELEN];
	char value[VALUELEN];
	byte commandid = 0, deviceid = 0, myvalue = 0;

	if (type == WebServer::HEAD) return;
	int response = 0;
	if (DEBUG_WEB) Serial.println(url_tail);

	if ((tail_complete) && (type == WebServer::POST)) {
		// Read Get params
		// post not working so Getting all
		if (strlen(url_tail)) {
			//server.printP(Parsed_tail_begin);
			while (strlen(url_tail)) {
				rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
				if (rc != URLPARAM_EOS) {
					if (DEBUG_WEB) Serial.print("name: ");
					if (DEBUG_WEB) Serial.print(name);
					if (DEBUG_WEB) Serial.print(" val: ");
					if (DEBUG_WEB) Serial.println(value);
					if (strcmp(name, "cmdid") == 0) commandid = atoi(value);
					if (strcmp(name, "devid") == 0) deviceid = atoi(value);
					if (strcmp(name, "value") == 0) myvalue = atoi(value);
				}
			}
			// Handle command
			//result = handleCommand(cmd, commandcounter, response);

			byte i;
			if (DEBUG_WEB) Serial.print("cmdid: ");
			if (DEBUG_WEB) Serial.print(commandid);
			if (DEBUG_WEB) Serial.print(" devid: ");
			if (DEBUG_WEB) Serial.print(deviceid);
			if (DEBUG_WEB) Serial.print(" value: ");
			if (DEBUG_WEB) Serial.println(myvalue);
			if (commandid > 0 && deviceid > 0) {
				i = findDevice(deviceid, FIND_BY_DEVICE_ID);
				if (i >= 0) {
					devices[i].setCommand(commandid);
					devices[i].setData(myvalue);
					//  Different response based on command, should be flexible and command independent, this is a bridge
					if (commandid == COMMAND_STATUSREQUEST) { // Now only Range
//						sendMessageRF(i, true);
						server.httpSuccess();
						printSensorValues(server.m_client, i);
					} else if (commandid == COMMAND_DEVICE_STATUS) { // Freemem etc
//						sendMessageRF(i, true);
						server.httpSuccess();
						printDeviceStatus(server, i);
					} else if (commandid == COMMAND_CALIBRATE) { // Simple response
//						sendMessageRF(i, true);
						server.httpSuccess();
						printSimpleResponse(server.m_client, i);
					} else {
//						sendMessageRF(i, false);
						server.httpSuccess();
					}
					return;
				} else {
					response = 100; //"Unknown device" + deviceid;
				}
			} else {
				response = 102; //"Unknown CommandID or DeviceID";
			}

		}
	}

	// Some error
	server.httpFail();
	server.printP(Page_start);
	server.printP(OpenTag);
	server.printP(sResponse);
	server.print(response, DEC);
	server.printP(CloseTag);
	server.printP(sResponse);
	server.printP(Page_end);
}

void setupWeb() {

	/* setup our default command that will be run when the user accesses
	 * a page NOT on the server */
	//webserver.setFailureCommand(&my_failCmd);
	/*This command  is called if you try to load /raw.html */
	webserver.addCommand("p.html", &processCmd);

	/* start the webserver */
	webserver.begin();
}

void updateWeb() {
	char buff[64];
	int len = 64;

	/* process incoming connections one at a time forever */
	webserver.processConnection(buff, &len);
}

static EthernetClient client;

void postMessage(byte deviceidx, byte cmdType) {

	if (!client.connected()) {
		client.connect(vlosite, 80);
		if (DEBUG_WEB) Serial.println("New connection");
	}
	if (!client.connected()) {
		delay(10);
		if (DEBUG_WEB) Serial.println("disconnecting.");
		client.stop();
		delay(100);
		client.connect(vlosite, 80);
		if (DEBUG_WEB) Serial.println("Retry connection");
	}
	int len = 0;
	if (client.connected()) {
		if (DEBUG_WEB) Serial.println("connected & posting");

		client.print("POST ");
		client.print(WEB_POST_TO_URL);
		client.println(" HTTP/1.1");
		client.println("Host: vlohome.homeip.net");
		client.println("Content-Type: text/html");
		client.println("Connection: close");
		client.print("Content-Length: ");
		//cmdType = PAYLOAD_TYPE_SIMPLE;
		if (cmdType == WEB_POST_TYPE_VALUES) {
			len = printSensorValues(client, deviceidx, true);
			client.println(len);
			client.println();
			printSensorValues(client, deviceidx);
		} else if (cmdType == WEB_POST_TYPE_SIMPLE) {
			len = printSimpleResponse(client, deviceidx, true);
			client.println(len);
			client.println();
			printSimpleResponse(client, deviceidx);
		}

		if (DEBUG_WEB) Serial.print("POST ");
		if (DEBUG_WEB) Serial.print(WEB_POST_TO_URL);
		if (DEBUG_WEB) Serial.println(" HTTP/1.1");
		if (DEBUG_WEB) Serial.println("Host: vlohome.homeip.net");
		if (DEBUG_WEB) Serial.println("Content-Type: text/html");
		if (DEBUG_WEB) Serial.println("Connection: close");
		if (DEBUG_WEB) Serial.print("Content-Length: ");
		if (DEBUG_WEB) Serial.println(len);
		if (DEBUG_WEB) Serial.println();
		if (DEBUG_WEB) Serial.println("?Values?");

		client.println();

		while (client.available()) {
			char c = client.read();
			if (DEBUG_WEB) Serial.print(c);
		}

	} else {
		if (DEBUG_WEB) Serial.println("Failed to connect-1");
	}

	delay(10);
	if (DEBUG_WEB) Serial.println("disconnecting.");
	client.stop();
//if (retry==0) Reset_AVR();
}

