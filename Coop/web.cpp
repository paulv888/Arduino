/*
 * Web.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "Web.h"

static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static uint8_t ip[] = { 192, 168, 2, 126 };
static uint8_t sn[] = { 255, 255, 255, 0 };
static uint8_t gw[] = { 192, 168, 2, 1 };
static uint8_t dns[] = { 192, 168, 2, 1 };
static uint8_t vlosite[] = { 192, 168, 2, 101 };

static EthernetServer server(80);      //server port
static EthernetClient client_recv;
static EthernetClient client_send;

//char *readString = "123456789012345678901234567890";
char readString[30] ;


P(TXTSBRACKETOPEN) = "{";
P(TXTSBRACKETCLOSE) = "}";
P(TXTQUOTE) = "\"";
P(TXTCOLON) = " : ";
P(TXTCOMMA) = " , ";
P(TXTDEVICEID) = "Device";
P(HEADER_OK) = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n";
P(HEADER_ERR) = "HTTP/1.1 422 ERROR\nContent-Type: text/html\nConnection: close\n";
P(HEADERPG2) = "<HTML>\n<HEAD>\n<meta name='apple-mobile-web-app-capable' content='yes' />\n<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />\n<link rel='stylesheet' type='text/css' href='http://vlohome.homeip.net/templates/protostar-mod/css/template.css' />\n<TITLE>Aynur's Chicken Coop</TITLE>\n</HEAD>";
P(HEADERPG3) = "<BODY class=\"site\">\n<div class=\"body\">\n<H1>Aynur's Beautiful Coop</H1>\n<br /><br />";
P(HEADERPGEND) = "</DIV></BODY>\n</HTML>";
P(TXTPOST) = "POST /cronjobs/70D455DC-ACB4-4525-8A85-E6009AE93AF4/a.php HTTP/1.1\nHost: vlohome.homeip.net\nContent-Type: text/html\nConnection: close\nContent-Length: ";
P(HEADER_ERR_MESS) = "ARD-COOP: Message parse error, check deviceID and commandID";

const char *varNames[] = {"Status","FreeMemory","Uptime","Temperature","Humidity","Command", "InOut", "ExtData"}; // PVTODO:: To Progmem???
#define STATUS 0
#define FREEMEMORY 1
#define UPTIME 2
#define INTERNALTEMP 3*/


int printP(const uint8_t clientsel, const prog_char *str, const bool getLen = false) {
	// copy data out of program memory into local storage, write out in
	// chunks of 32 bytes to avoid extra short TCP/IP packets

	if (!getLen) {

		uint8_t buffer[32];					// PVTODO:: Share buffer
		size_t bufferEnd = 0;
		while ((buffer[bufferEnd++] = pgm_read_byte(str++))) {
			if (bufferEnd == 32) {
				if (clientsel == COMMAND_IO_RECV) {
					client_recv.write(buffer, 32);
				} else {
					client_send.write(buffer, 32);
				}
	     		if (DEBUG_WEB) Serial.write(buffer, 32);
				bufferEnd = 0;
			}
		}

		// write out everything left but trailing NUL
		if (bufferEnd > 1) {
			if (clientsel == COMMAND_IO_RECV) {
				client_recv.write(buffer, bufferEnd - 1);
			} else {
				client_send.write(buffer, bufferEnd - 1);
			}
     		if (DEBUG_WEB) Serial.write(buffer, bufferEnd - 1);
		}
	}
	return strlen_P(str);
}

int printVstr(const uint8_t clientsel, const char *variable, const bool getLen = false) {
	if (!getLen) {
		if (clientsel == COMMAND_IO_RECV) {
			client_recv.print(variable);
		} else {
			client_send.print(variable);
		}
		if (DEBUG_WEB) Serial.print(variable);
	}
	return strlen(variable);
}

int printV(const uint8_t clientsel, const int variable, const bool getLen = false) {
	if (!getLen) {
		if (clientsel == COMMAND_IO_RECV) {
			client_recv.print(variable);
		} else {
			client_send.print(variable);
		}
		if (DEBUG_WEB) Serial.print(variable);
	}
	char buffer[16];
	itoa(variable, buffer, 10);
	uint8_t a = strlen(buffer);
	return a;
}

int printResponse(const uint8_t clientsel, const uint8_t deviceidx, const bool getLen = false) {

	int len = 0;

	len += printP(clientsel, TXTSBRACKETOPEN, getLen);

	len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTDEVICEID, getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTCOLON, getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printV(clientsel, mdevices[deviceidx].getDeviceid(), getLen);
	len += printP(clientsel, TXTQUOTE, getLen);

	for (uint8_t i = 0 ; i < MAX_NUMBER_OF_VALUES && mdevices[deviceidx].getValueTypebyInd(i)!=ERROR ;  i++ ) {
		len += printP(clientsel, TXTCOMMA, getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printVstr(clientsel, varNames[mdevices[deviceidx].getValueTypebyInd(i)] , getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTCOLON, getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printVstr(clientsel, mdevices[deviceidx].getValuebyInd(i), getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
	}

	len += printP(clientsel, TXTSBRACKETCLOSE, getLen);

	return len;
}

void setupWeb(){
	// initialize the Ethernet adapter
	Ethernet.begin(mac, ip, dns, gw, sn );
	server.begin();
	if (DEBUG_WEB) Serial.print("Coop server is at ");
	if (DEBUG_WEB) Serial.println(Ethernet.localIP());
}

void updateWeb(){
/*Serial.print(" Web 1 ");
check_mem();
Serial.print(" heapptr ");
Serial.print((long)heapptr);
Serial.print(" stackptr ");
Serial.println((long)stackptr);
*/
	client_recv = server.available();
	uint8_t cptr;
	if (client_recv) {
		while (client_recv.connected()) {
			if (client_recv.available()) {
				char c = client_recv.read();
				//read char by char HTTP request

				if (cptr < 29) {
					readString[cptr++] = c;
				} else {
					cptr = 0;
				}
				readString[cptr] = '\0';

				//if HTTP request has ended
				if (c == '\n') {
					if (DEBUG_WEB) Serial.println(readString); //print to serial monitor for debuging

					if (strstr(readString,"GET / ")) { 		// Root requested, then give page, else try to parse post parameters
	    	     		printP(COMMAND_IO_RECV, HEADER_OK);
						client_recv.println();
	    	     		printP(COMMAND_IO_RECV, HEADERPG2);
	    	     		printP(COMMAND_IO_RECV, HEADERPG3);

	    	     		client_recv.println("<H6>");
						for (uint8_t d = 0 ; d<DEVICE_COUNT ;  d++ ) {
    						client_recv.println("<H3>");
							client_recv.println(mdevices[d].getName());
							client_recv.println("</H3>");
    						client_recv.println("<H6>");
    						for (uint8_t i = 0 ;i < MAX_NUMBER_OF_VALUES && mdevices[d].getValueTypebyInd(i)!=ERROR ;  i++ ) {
    							printVstr(COMMAND_IO_RECV, varNames[mdevices[d].getValueTypebyInd(i)]);
    							printP(COMMAND_IO_RECV, TXTCOLON);
    							printVstr(COMMAND_IO_RECV, mdevices[d].getValuebyInd(i));
    							client_recv.println("<br />");
    						}
							client_recv.println("<br />");
							client_recv.println("</H6>");		// PVTODO:: check if still loosing bytes for these call, if not to progmem
						}
						printP(COMMAND_IO_RECV, HEADERPGEND);
					} else {												// parse

						// parse url POST /d/203/c/23/v/12 HTTP/1.1
						int deviceID = 0;
						int commandID = 0;
						int commandvalue = 0;
						//readString = strtok(readString, " "); // POST /d/203/c/23/v/12 HTTP/1.1
						char * token = strtok(readString, " "); // POST /d/203/c/23/v/12 HTTP/1.1
						uint8_t deviceIdx = ERROR;
						uint8_t result = ERROR;
						if (strcmp(token,"POST") == 0) {
							token = strtok(NULL, " "); // Second part
							token = strtok(token, "/"); // d
							token = strtok(NULL, "/"); // deviceID
							deviceID = atoi(token);
							token = strtok(NULL, "/"); // c
							token = strtok(NULL, "/"); // commandID
							commandID = atoi(token);
							token = strtok(NULL, "/"); // value ?
							if (token != NULL) {
								token = strtok(NULL, "/"); // value ?
								commandvalue = atoi(token);
							}
							uint8_t (*handler)(const uint8_t ,const int,const int);

							deviceIdx = findDeviceIndex(deviceID);
							if (DEBUG_WEB) Serial.print("deviceIdx");
							if (DEBUG_WEB) Serial.println(deviceIdx);

							if (deviceIdx != ERROR) {
								handler = mdevices[deviceIdx].commandHandler;
								result = (*handler)(deviceIdx, commandID, commandvalue);
							} // return error?
						}

						if ( deviceIdx == ERROR || result == ERROR) {
							printP(COMMAND_IO_RECV, HEADER_ERR);
							client_recv.println();
							printP(COMMAND_IO_RECV, HEADER_ERR_MESS);
						} else {
							printP(COMMAND_IO_RECV, HEADER_OK);
							client_recv.println();
							printResponse(COMMAND_IO_RECV, deviceIdx);
						}
					}
					delay(1);
					//stopping client
					client_recv.stop();
					//clearing string for next read
					cptr = 0;
					readString[cptr] = '\0';
				}
			}
		}
	}
}

void postMessage(const uint8_t deviceidx) {

	if (client_send.connect(vlosite, 80)) {
	if (DEBUG_WEB) Serial.println("New connection");

		int len = 0;

		printP(COMMAND_IO_SEND, TXTPOST);
		len = printResponse(COMMAND_IO_SEND, deviceidx, true);
		client_send.println(len);
		if (DEBUG_WEB) Serial.println(len);
		client_send.println();
		if (DEBUG_WEB) Serial.println();
		printResponse(COMMAND_IO_SEND, deviceidx);
		client_send.println();

		delay (3);
		while (client_send.available()) {
			char c = client_send.read();
			if (DEBUG_WEB) Serial.print(c);
		}

	} else {
		if (DEBUG_WEB) Serial.println("Failed to connect-1");
	}

	delay(1);
	if (DEBUG_WEB) Serial.println("disconnecting.");
	client_send.stop();
//if (retry==0) Reset_AVR();
}

