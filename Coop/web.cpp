/*
 * Web.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "Web.h"

static byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static byte ip[] = { 192, 168, 2, 126 };
static byte sn[] = { 255, 255, 255, 0 };
static byte gw[] = { 192, 168, 2, 1 };
static byte dns[] = { 192, 168, 2, 1 };
static byte vlosite[] = { 192, 168, 2, 101 };

static EthernetServer server(80);      //server port
static EthernetClient client_recv;
static EthernetClient client_send;

//char *readString = "123456789012345678901234567890";
char readString[MAX_POST_PARAMS] ;


P(TXTSBRACKETOPEN) = "{";
P(TXTSBRACKETCLOSE) = "}";
P(TXTQUOTE) = "\"";
P(TXTCOLON) = " : ";
P(TXTCOMMA) = " , ";
P(TXTDEVICEID) = "Device";
P(HEADER_OK) = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n";
P(HEADER_ERR) = "HTTP/1.1 422 ERROR\nContent-Type: text/html\nConnection: close\n";
P(HEADERPG2) = "<HTML>\n<HEAD>\n<meta name='apple-mobile-web-app-capable' content='yes' />\n<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />\n<link rel='stylesheet' type='text/css' href='http://vlohome.homeip.net/templates/protostar-mod/css/template.css' />\n<TITLE>Aynur's Beautiful Coop</TITLE>\n</HEAD>";
P(HEADERPG3) = "<BODY class=\"site\">\n<div class=\"body\">\n<H1>Aynur's Beautiful Coop</H1>\n<br /><br />";
P(HEADERPGEND) = "</DIV></BODY>\n</HTML>";
P(TXTPOST) = "POST /cronjobs/70D455DC-ACB4-4525-8A85-E6009AE93AF4/a.php HTTP/1.1\nHost: vlohome.homeip.net\nContent-Type: text/html\nConnection: close\nContent-Length: ";
P(HEADER_ERR_MESS) = "ARD-COOP: Message parse error, check deviceID and commandID";
P(AOPEN) = "<";
P(ACLOSE) = ">";
P(H3) = "H3";
P(H6) = "H6";
P(BR) = "BR";
P(SLASH) = "/";
P(TXTSTATUS) = "Status";
P(TXTVALUE) = "Value";
P(TXTCOMMAND) = "Command";
P(TXTINOUT) = "InOut";
P(TXTEXTDATA) = "ExtData";


int printP(const byte clientsel, const prog_char *str, const bool getLen = false) {
	// copy data out of program memory into local storage, write out in
	// chunks of 32 bytes to avoid extra short TCP/IP packets

	if (!getLen) {

		byte buffer[32];					// PVTODO:: Share buffer
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

int printVstr(const byte clientsel, const char *variable, const bool getLen = false) {
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

int printV(const byte clientsel, const int variable, const bool getLen = false) {
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
	byte a = strlen(buffer);
	return a;
}

int printResponse(const byte clientsel, const byte deviceidx, const byte JSON, const bool getLen = false) {

	int len = 0;

	if (JSON) len += printP(clientsel, TXTSBRACKETOPEN, getLen);

	// DeviceID
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTDEVICEID, getLen);
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTCOLON, getLen);
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	len += printV(clientsel, mdevices[deviceidx].getDeviceid(), getLen);
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);

	// CommandID
	if (JSON) {
		len += printP(clientsel, TXTCOMMA, getLen);
	}
	else {
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
	}
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTCOMMAND , getLen);
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTCOLON, getLen);
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	len += printVstr(clientsel, mdevices[deviceidx].getCommand(), getLen);
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);

	// Status
	if (JSON) {
		len += printP(clientsel, TXTCOMMA, getLen);
	}
	else {
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
	}
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTSTATUS , getLen);
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTCOLON, getLen);
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	len += printVstr(clientsel, mdevices[deviceidx].getStatus(), getLen);
	if (JSON) len += printP(clientsel, TXTQUOTE, getLen);

	// Value
	if (mdevices[deviceidx].commandvalue != NO_VALUE) {
		if (JSON) {
			len += printP(clientsel, TXTCOMMA, getLen);
		}
		else {
			printP(clientsel, AOPEN);
			printP(clientsel, BR);
			printP(clientsel, SLASH);
			printP(clientsel, ACLOSE);
		}
		if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTVALUE , getLen);
		if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTCOLON, getLen);
		if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
		len += printVstr(clientsel, mdevices[deviceidx].getValue(), getLen);
		if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	}

	// InOut
	if (JSON) {
		len += printP(clientsel, TXTCOMMA, getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTINOUT , getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTCOLON, getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printVstr(clientsel, "1", getLen);
		if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
	}

	// ExtData
	if (printVstr(clientsel, mdevices[deviceidx].getExtData(), true) > 0) {
		if (JSON) {
			len += printP(clientsel, TXTCOMMA, getLen);
		}
		else {
			printP(clientsel, AOPEN);
			printP(clientsel, BR);
			printP(clientsel, SLASH);
			printP(clientsel, ACLOSE);
		}
		if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTEXTDATA , getLen);
		if (JSON) len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTCOLON, getLen);
		len += printVstr(clientsel, mdevices[deviceidx].getExtData(), getLen);
	}

	if (JSON) len += printP(clientsel, TXTSBRACKETCLOSE, getLen);
	if (DEBUG_WEB) Serial.println();

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
//if (DEBUG_MEMORY) printMem(" Web 1 ");

client_recv = server.available();
	byte cptr;
	const char *slash = "/";
	const char *space = " ";

	if (client_recv) {
		while (client_recv.connected()) {
			if (client_recv.available()) {
				char c = client_recv.read();
				//read char by char HTTP request

				if (cptr < MAX_POST_PARAMS-1) {
					readString[cptr++] = c;
				} else {
					cptr = 0;
				}
				readString[cptr] = '\0';

				//if HTTP request has ended
				if (c == '\n') {
					if (DEBUG_WEB) Serial.println(readString); //print to serial monitor for debuging

					if (strstr(readString,"GET / ")) { 		// Root requested, then give page, else try to parse post parameters
						if (DEBUG_MEMORY) printMem("WebG ");
	    	     		printP(COMMAND_IO_RECV, HEADER_OK);
						client_recv.println();
	    	     		printP(COMMAND_IO_RECV, HEADERPG2);
	    	     		printP(COMMAND_IO_RECV, HEADERPG3);

						for (byte d = 0 ; d<DEVICE_COUNT ;  d++ ) {
		    	     		printP(COMMAND_IO_RECV, AOPEN);
		    	     		printP(COMMAND_IO_RECV, H3);					// <H3>
		    	     		printP(COMMAND_IO_RECV, ACLOSE);
							client_recv.println(mdevices[d].getName());
		    	     		printP(COMMAND_IO_RECV, AOPEN);
		    	     		printP(COMMAND_IO_RECV, SLASH);
		    	     		printP(COMMAND_IO_RECV, H3);					// </H3>
		    	     		printP(COMMAND_IO_RECV, ACLOSE);
		    	     		printP(COMMAND_IO_RECV, AOPEN);
		    	     		printP(COMMAND_IO_RECV, H6);					// <h6>
		    	     		printP(COMMAND_IO_RECV, ACLOSE);

							printResponse(COMMAND_IO_RECV, d, false, false);

		    	     		printP(COMMAND_IO_RECV, AOPEN);
		    	     		printP(COMMAND_IO_RECV, BR);
		    	     		printP(COMMAND_IO_RECV, SLASH);
		    	     		printP(COMMAND_IO_RECV, ACLOSE);
		    	     		printP(COMMAND_IO_RECV, AOPEN);
		    	     		printP(COMMAND_IO_RECV, SLASH);
		    	     		printP(COMMAND_IO_RECV, H6);					// </h6>
		    	     		printP(COMMAND_IO_RECV, ACLOSE);
						}
						printP(COMMAND_IO_RECV, HEADERPGEND);
					} else {												// parse

						// parse url POST /d/203/c/23/v/12 HTTP/1.1
						if (DEBUG_MEMORY) printMem("WebP ");
						int deviceID = 0;
						int commandID = 0;
						int commandvalue = 0;
						//readString = strtok(readString, " "); // POST /d/203/c/23/v/12 HTTP/1.1
						char * token = strtok(readString, space); // POST /d/203/c/23/v/12 HTTP/1.1
						byte deviceIdx = ERROR;
						byte result = ERROR;
						if (strcmp(token,"POST") == 0) {
							token = strtok(NULL, space); // Second part
							token = strtok(token, slash); // d
							token = strtok(NULL, slash); // deviceID
							deviceID = atoi(token);
							token = strtok(NULL, slash); // c
							token = strtok(NULL, slash); // commandID
							commandID = atoi(token);
							token = strtok(NULL, slash); // v
							if (token != NULL) {
								token = strtok(NULL, slash); // value ?
								commandvalue = atoi(token);
							}
							byte (*handler)(const byte ,const int,const int);

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
							if ( result == HNDLR_WRITE_RESULT) printResponse(COMMAND_IO_RECV, deviceIdx, true, false);
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


void postMessage(const byte deviceidx) {

	if (DEBUG_MEMORY) printMem("Post");

	if (client_send.connect(vlosite, 80)) {
	if (DEBUG_WEB) Serial.println("NewC");

		int len = 0;

		printP(COMMAND_IO_SEND, TXTPOST);
		len = printResponse(COMMAND_IO_SEND, deviceidx, true, true);
		client_send.println(len);
		if (DEBUG_WEB) Serial.println(len);
		client_send.println();
		if (DEBUG_WEB) Serial.println();
		printResponse(COMMAND_IO_SEND, deviceidx, true,  false);
		client_send.println();

		delay (3);
		while (client_send.available()) {
			char c = client_send.read();
			if (DEBUG_WEB) Serial.print(c);
		}

	} else {
		if (DEBUG_WEB) Serial.println("Fail");
	}

	delay(1);
	if (DEBUG_WEB) Serial.println("Disc");
	client_send.stop();
//if (retry==0) Reset_AVR();
}

