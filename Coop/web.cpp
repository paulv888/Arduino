/*
 * Web.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "Web.h"

static byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static byte ip[] = { 192, 168, 2, 126 };
static byte vlosite[] = { 192, 168, 2, 101 };

static EthernetServer server(80);      //server port
static EthernetClient client_recv;
static EthernetClient client_send;


P(TXTSBRACKETOPEN) = "{";
P(TXTSBRACKETCLOSE) = "}";
P(TXTQUOTE) = "\"";
P(TXTCOLON) = " : ";
P(TXTCOMMA) = " , ";
P(TXTDEVICEID) = "Device";
P(HEADER_OK) = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n";
P(HEADER_ERR) = "HTTP/1.1 422 ERROR\nContent-Type: text/html\nConnection: close\n";
P(HEADERPG2) = "<HTML>\n<HEAD>\n<meta name='apple-mobile-web-app-capable' content='yes' />\n<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />\n<link rel='stylesheet' type='text/css' href='http://vlohome.homeip.net/templates/protostar-mod/css/template.css' />\n<TITLE>Aynur's Coop</TITLE>\n</HEAD>";
P(HEADERPG3) = "<BODY class=\"site\">\n<div class=\"body\">\n<H1>Aynur's Coop</H1>\n";
P(HEADERPGEND) = "</DIV></BODY>\n</HTML>";
P(TXTPOST) = "POST /cronjobs/70D455DC-ACB4-4525-8A85-E6009AE93AF4/a.php HTTP/1.1\nHost: vlohome.homeip.net\nContent-Type: text/html\nConnection: close\nContent-Length: ";
P(HEADER_ERR_MESS) = "ARD-COOP: Message parse error, check deviceID and commandID";
P(AOPEN) = "<";
P(ACLOSE) = ">";
P(ANBSP) = "&nbsp;&nbsp;&nbsp;";
P(H3) = "H3";
P(SPAN) = "SPAN";
P(BR) = "BR";
P(SLASH) = "/";
P(TXTSTATUS) = "Status";
P(TXTVALUE) = "Value";
P(TXTCOMMAND) = "Command";
P(TXTINOUT) = "InOut";
P(TXTEXTDATA) = "ExtData";
P(TXTPAR1) = "Parameter 1";
P(TXTPAR2) = "Parameter 2";
P(TXTPAR3) = "Parameter 3";
P(TXTPAR4) = "Parameter 4";
P(TXTPAR5) = "Parameter 5";
P(TXTIND) = "Index";
P(TXTDEVIND) = "Input Device Index";
P(TXTPIN) = "Pin";
P(TXTTYPE) = "Type";
P(DEV_0) = "Arduino-7";
P(DEV_1) = "Door-2345";
P(DEV_2) = "Heat-A0/A3";
P(DEV_3) = "Fan-6/0";
P(DEV_4) = "Water-A2";
P(DEV_5) = "Dark-A1";
P(DEV_6) = "Light-A4";
P(DEV_7) = "RedLgt-A5";
P(DEV_8) = "DHT22-6";
P(DEV_9) = "NTC A0" ;


int printP(const byte clientsel, const char *str, const bool getLen = false) {
	// copy data out of program memory into local storage, write out in
	// chunks of 32 bytes to avoid extra short TCP/IP packets

	if (!getLen) {

		size_t bufferEnd = 0;
		while ((temp[bufferEnd++] = pgm_read_byte(str++))) {
			if (bufferEnd == MAX_STRING_LEN) {
				if (clientsel == COMMAND_IO_RECV) {
					client_recv.write(temp, MAX_STRING_LEN);
				} else {
					client_send.write(temp, MAX_STRING_LEN);
				}
	     		if (DEBUG_WEB) Serial.write(temp, MAX_STRING_LEN);
				bufferEnd = 0;
			}
		}

		// write out everything left but trailing NUL
		if (bufferEnd > 1) {
			if (clientsel == COMMAND_IO_RECV) {
				client_recv.write(temp, bufferEnd - 1);
			} else {
				client_send.write(temp, bufferEnd - 1);
			}
     		if (DEBUG_WEB) Serial.write(temp, bufferEnd - 1);
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
	itoa(variable, temp, 10);
	byte a = strlen(temp);
	return a;
}

int printResponse(const byte clientsel, const byte deviceIdx, const bool getLen = false) {

	int len = 0;

	len += printP(clientsel, TXTSBRACKETOPEN, getLen);

	// DeviceID
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTDEVICEID, getLen);
	printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTCOLON, getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printV(clientsel, mdevices[deviceIdx].getDeviceID(), getLen);
	len += printP(clientsel, TXTQUOTE, getLen);

	// CommandID
	len += printP(clientsel, TXTCOMMA, getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTCOMMAND , getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTCOLON, getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printVstr(clientsel, mdevices[deviceIdx].getCommand(), getLen);
	len += printP(clientsel, TXTQUOTE, getLen);

	// Status
	len += printP(clientsel, TXTCOMMA, getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTSTATUS , getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTCOLON, getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printVstr(clientsel, mdevices[deviceIdx].getStatus(), getLen);
	len += printP(clientsel, TXTQUOTE, getLen);

	// Value
	if (mdevices[deviceIdx].commandvalue != 0) {
		len += printP(clientsel, TXTCOMMA, getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTVALUE , getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTCOLON, getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printVstr(clientsel, mdevices[deviceIdx].getValue(), getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
	}

	// InOut
	len += printP(clientsel, TXTCOMMA, getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTINOUT , getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printP(clientsel, TXTCOLON, getLen);
	len += printP(clientsel, TXTQUOTE, getLen);
	len += printVstr(clientsel, "1", getLen);
	len += printP(clientsel, TXTQUOTE, getLen);

	// ExtData
	if (printVstr(clientsel, mdevices[deviceIdx].getExtData(), true) > 0) {
		len += printP(clientsel, TXTCOMMA, getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTEXTDATA , getLen);
		len += printP(clientsel, TXTQUOTE, getLen);
		len += printP(clientsel, TXTCOLON, getLen);
		len += printVstr(clientsel, mdevices[deviceIdx].getExtData(), getLen);
	}

	len += printP(clientsel, TXTSBRACKETCLOSE, getLen);

	if (DEBUG_WEB) Serial.println();

	return len+1;			// WHYYYYYYYYYYYY was not needed before refactor on 1/13/2015
}

void printPage(const byte clientsel, const byte deviceIdx) {

	// Index
	printP(clientsel, TXTIND);
	printP(clientsel, TXTCOLON);
	printV(clientsel, deviceIdx);
	printP(clientsel, ANBSP);

	// DeviceID
	printP(clientsel, TXTDEVICEID);
	printP(clientsel, TXTCOLON);
	printV(clientsel, mdevices[deviceIdx].getDeviceID());

	// Type
	printP(clientsel, ANBSP);
	if (mdevices[deviceIdx].getType() != 0) {
		printP(clientsel, TXTTYPE);
		printP(clientsel, TXTCOLON);
		printV(clientsel, mdevices[deviceIdx].getType());
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
	}

	// CommandID
	printP(clientsel, TXTCOMMAND );
	printP(clientsel, TXTCOLON);
	printVstr(clientsel, mdevices[deviceIdx].getCommand());

	// Status
	printP(clientsel, ANBSP);
	printP(clientsel, TXTSTATUS );
	printP(clientsel, TXTCOLON);
	printVstr(clientsel, mdevices[deviceIdx].getStatus());

	// Value
	if (mdevices[deviceIdx].commandvalue != 0) {
		printP(clientsel, ANBSP);
		printP(clientsel, TXTVALUE );
		printP(clientsel, TXTCOLON);
		printVstr(clientsel, mdevices[deviceIdx].getValue());
	}

	// ExtData
	if (printVstr(clientsel, mdevices[deviceIdx].getExtData(), true) > 0) {
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
		printP(clientsel, TXTEXTDATA );
		printP(clientsel, TXTCOLON);
		printVstr(clientsel, mdevices[deviceIdx].getExtData());
	}


	printP(clientsel, AOPEN);
	printP(clientsel, BR);
	printP(clientsel, SLASH);
	printP(clientsel, ACLOSE);
	if (EEPROMReadInt(PARAMS(deviceIdx, 1)) !=  FFFF) {
		// Parameter 1
		printP(clientsel, TXTPAR1);
		printP(clientsel, TXTCOLON);
		printV(clientsel, EEPROMReadInt(PARAMS(deviceIdx, 1)));
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
	}
	if (EEPROMReadInt(PARAMS(deviceIdx, 2)) !=  FFFF) {
		// Parameter 2
		printP(clientsel, TXTPAR2);
		printP(clientsel, TXTCOLON);
		printV(clientsel, EEPROMReadInt(PARAMS(deviceIdx, 2)));
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
	}
	if (EEPROMReadInt(PARAMS(deviceIdx, 3)) !=  FFFF) {
		// Parameter 3
		printP(clientsel, TXTPAR3);
		printP(clientsel, TXTCOLON);
		printV(clientsel, EEPROMReadInt(PARAMS(deviceIdx, 3)));
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
	}
	if (EEPROMReadInt(PARAMS(deviceIdx, 4)) !=  FFFF) {
		// Parameter 3
		printP(clientsel, TXTPAR4);
		printP(clientsel, TXTCOLON);
		printV(clientsel, EEPROMReadInt(PARAMS(deviceIdx, 4)));
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
	}
	if (EEPROMReadInt(PARAMS(deviceIdx, 5)) !=  FFFF) {
		// Parameter 5
		printP(clientsel, TXTPAR5);
		printP(clientsel, TXTCOLON);
		printV(clientsel, EEPROMReadInt(PARAMS(deviceIdx, 5)));
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
	}

	// In deviceIdx
	if (mdevices[deviceIdx].getInput() != 0) {
		printP(clientsel, TXTDEVIND);
		printP(clientsel, TXTCOLON);
		printV(clientsel, mdevices[deviceIdx].getInput());
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
	}

	// Pin
	if (mdevices[deviceIdx].getPin() != 0) {
		printP(clientsel, TXTPIN);
		printP(clientsel, TXTCOLON);
		printV(clientsel, mdevices[deviceIdx].getPin());
		printP(clientsel, AOPEN);
		printP(clientsel, BR);
		printP(clientsel, SLASH);
		printP(clientsel, ACLOSE);
	}


	if (DEBUG_WEB) Serial.println();

	return;
}

void setupWeb(){
	// initialize the Ethernet adapter
	Ethernet.begin(mac, ip );
	server.begin();
	if (DEBUG_WEB) Serial.print("Coop server is at ");
	if (DEBUG_WEB) Serial.println(Ethernet.localIP());
}

byte findDeviceIndex(const int _deviceID) {
	byte found = ERROR;
	for (byte i = 0; i < DEVICE_COUNT; i++) {
		if (mdevices[i].getDeviceID() == _deviceID) {
			return i;
		}
	}
	return found;
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
				if (cptr < MAX_STRING_LEN-1) {
					temp[cptr++] = c;
				} else {
					cptr = 0;
				}
				temp[cptr] = '\0';

				//if HTTP request has ended
				if (c == '\n') {
					if (DEBUG_WEB) Serial.println(temp); //print to serial monitor for debuging

					if (strstr(temp,"GET / ")) { 		// Root requested, then give page, else try to parse post parameters
						if (DEBUG_MEMORY) printMem("WebG ");
	    	     		printP(COMMAND_IO_RECV, HEADER_OK);
						client_recv.println();
	    	     		printP(COMMAND_IO_RECV, HEADERPG2);
	    	     		printP(COMMAND_IO_RECV, HEADERPG3);

						for (byte deviceIdx = 0 ; deviceIdx < DEVICE_COUNT ;  deviceIdx++ ) {
							mdevices[deviceIdx].readInput();
		    	     		printP(COMMAND_IO_RECV, AOPEN);
		    	     		printP(COMMAND_IO_RECV, H3);					// <H3>
		    	     		printP(COMMAND_IO_RECV, ACLOSE);
							switch (deviceIdx) {
							case 0:
								printP(COMMAND_IO_RECV, DEV_0);
								break;
							case 1:
								printP(COMMAND_IO_RECV, DEV_1);
								break;
							case 2:
								printP(COMMAND_IO_RECV, DEV_2);
								break;
							case 3:
								printP(COMMAND_IO_RECV, DEV_3);
								break;
							case 4:
								printP(COMMAND_IO_RECV, DEV_4);
								break;
							case 5:
								printP(COMMAND_IO_RECV, DEV_5);
								break;
							case 6:
								printP(COMMAND_IO_RECV, DEV_6);
								break;
							case 7:
								printP(COMMAND_IO_RECV, DEV_7);
								break;
							case 8:
								printP(COMMAND_IO_RECV, DEV_8);
								break;
							case 9:
								printP(COMMAND_IO_RECV, DEV_9);
								break;
							}
		    	     		printP(COMMAND_IO_RECV, AOPEN);
		    	     		printP(COMMAND_IO_RECV, SLASH);
		    	     		printP(COMMAND_IO_RECV, H3);					// </H3>
		    	     		printP(COMMAND_IO_RECV, ACLOSE);
		    	     		printP(COMMAND_IO_RECV, AOPEN);
		    	     		printP(COMMAND_IO_RECV, SPAN);					// <SPAN>
		    	     		printP(COMMAND_IO_RECV, ACLOSE);

							printPage(COMMAND_IO_RECV, deviceIdx);

		    	     		printP(COMMAND_IO_RECV, AOPEN);
		    	     		printP(COMMAND_IO_RECV, BR);
		    	     		printP(COMMAND_IO_RECV, SLASH);
		    	     		printP(COMMAND_IO_RECV, ACLOSE);
		    	     		printP(COMMAND_IO_RECV, AOPEN);
		    	     		printP(COMMAND_IO_RECV, SLASH);
		    	     		printP(COMMAND_IO_RECV, SPAN);					// </SPAN>
		    	     		printP(COMMAND_IO_RECV, ACLOSE);
						}
						printP(COMMAND_IO_RECV, HEADERPGEND);
					} else {												// parse

						// parse url POST /d/203/c/23/v/12 HTTP/1.1
						if (DEBUG_MEMORY) printMem("WebP ");
						int deviceID = 0;
						int commandID = 0;
						int commandvalue = 0;
						//temp = strtok(temp, " "); // POST /d/203/c/23/v/12 HTTP/1.1
						char * token = strtok(temp, space); // POST /d/203/c/23/v/12 HTTP/1.1
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
							deviceIdx = findDeviceIndex(deviceID);
							if (DEBUG_WEB) Serial.print("Idx ");
							if (DEBUG_WEB) Serial.println(deviceIdx);

							if (deviceIdx != ERROR) {
								result = deviceCommandHandler(deviceIdx, commandID, false, commandvalue);
							} // return error?
						}

						if ( deviceIdx == ERROR || result == ERROR) {
							printP(COMMAND_IO_RECV, HEADER_ERR);
							client_recv.println();
							printP(COMMAND_IO_RECV, HEADER_ERR_MESS);
						} else {
							printP(COMMAND_IO_RECV, HEADER_OK);
							client_recv.println();
							if ( result == HNDLR_WRITE_RESULT) printResponse(COMMAND_IO_RECV, deviceIdx, false);
						}
					}
					delay(1);
					//stopping client
					client_recv.stop();
					//clearing string for next read
					cptr = 0;
					temp[cptr] = '\0';
				}
			}
		}
	}
}


void postMessage(const byte deviceIdx) {

	if (DEBUG_MEMORY) printMem("Post ");

	if (client_send.connect(vlosite, 80)) {
	if (DEBUG_WEB) Serial.println("NewC");

		int len = 0;

		printP(COMMAND_IO_SEND, TXTPOST);
		len = printResponse(COMMAND_IO_SEND, deviceIdx, true);
		client_send.println(len);
		if (DEBUG_WEB) Serial.println(len);
		client_send.println();
		if (DEBUG_WEB) Serial.println();
		printResponse(COMMAND_IO_SEND, deviceIdx, false);
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


