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

char readString[100];


P(TXTSBRACKETOPEN) = "{";
P(TXTSBRACKETCLOSE) = "}";
P(TXTQUOTE) = "\"";
P(TXTCOLON) = " : ";
P(TXTCOMMA) = " , ";
P(TXTDEVICEID) = "Device";
P(HEADER1) = "HTTP/1.1 200 OK\nContent-Type: text/html\n";
P(HEADER2) = "<HTML>\n<HEAD>\n<meta name='apple-mobile-web-app-capable' content='yes' />\n<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />\n<link rel='stylesheet' type='text/css' href='http://vlohome.homeip.net/templates/protostar-mod/css/template.css' />\n<TITLE>Aynur's Chicken Coop</TITLE>\n</HEAD>";
P(TXTPOST) = "POST /cronjobs/70D455DC-ACB4-4525-8A85-E6009AE93AF4/a.php HTTP/1.1\nHost: vlohome.homeip.net\nContent-Type: text/html\nConnection: close\nContent-Length: ";


/*#define NO_VALUE -1
#define STATUS 0
#define FREEMEMORY 1
#define UPTIME 2
#define INTERNALTEMP 3*/
static const char *varNames[] = {"Status","FreeMemory","Uptime","Temperature","Humidity"};


int printP(EthernetClient client, const prog_char *str, bool getLen = false) {
	// copy data out of program memory into local storage, write out in
	// chunks of 32 bytes to avoid extra short TCP/IP packets

	if (!getLen) {

		uint8_t buffer[32];
		size_t bufferEnd = 0;
		while (buffer[bufferEnd++] = pgm_read_byte(str++)) {
			if (bufferEnd == 32) {
				client.write(buffer, 32);
	     		if (DEBUG_WEB) Serial.write(buffer, 32);
				bufferEnd = 0;
			}
		}

		// write out everything left but trailing NUL
		if (bufferEnd > 1) {
			client.write(buffer, bufferEnd - 1);
     		if (DEBUG_WEB) Serial.write(buffer, bufferEnd - 1);
		}
	}
	return strlen_P(str);
}

int printVstr(EthernetClient client, const char *variable, bool getLen = false) {
	if (!getLen) {
		client.print(variable);
		if (DEBUG_WEB) Serial.print(variable);
	}
	return strlen(variable);
}

int printV(EthernetClient client, int variable, bool getLen = false) {
	if (!getLen) {
		client.print(variable);
		if (DEBUG_WEB) Serial.print(variable);
	}
	char buffer[16];
	itoa(variable, buffer, 10);
	int a = strlen(buffer);
	return a;
}

int printResponse(EthernetClient client, byte deviceidx, bool getLen = false) {

	int len = 0;

	len += printP(client, TXTSBRACKETOPEN, getLen);

	len += printP(client, TXTQUOTE, getLen);
	len += printP(client, TXTDEVICEID, getLen);
	len += printP(client, TXTQUOTE, getLen);
	len += printP(client, TXTCOLON, getLen);
	len += printP(client, TXTQUOTE, getLen);
	len += printV(client, devices[deviceidx].getDeviceid(), getLen);
	len += printP(client, TXTQUOTE, getLen);

	for (int i = 0 ; i < MAX_NUMBER_OF_VALUES && devices[deviceidx].getValueTypebyInd(i)!=NO_VALUE ;  i++ ) {
		len += printP(client, TXTCOMMA, getLen);
		len += printP(client, TXTQUOTE, getLen);
		len += printVstr(client, varNames[devices[deviceidx].getValueTypebyInd(i)] , getLen);
		len += printP(client, TXTQUOTE, getLen);
		len += printP(client, TXTCOLON, getLen);
		len += printP(client, TXTQUOTE, getLen);
		len += printVstr(client, devices[deviceidx].getValuebyInd(i), getLen);
		len += printP(client, TXTQUOTE, getLen);
	}

	len += printP(client, TXTSBRACKETCLOSE, getLen);

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
	  EthernetClient client = server.available();
	  int cptr;
	  if (client) {
	    while (client.connected()) {
	      if (client.available()) {
	        char c = client.read();

	        //read char by char HTTP request
	        if (cptr < 99) {
	          //store characters to string
	          readString[cptr++] = c;
	          readString[cptr] = '\0';
	          //Serial.print(c);
	         } else {
	        	 cptr = 0;
	        	 readString[cptr] = '\0';
	         }

	         //if HTTP request has ended
	         if (c == '\n') {
	     		if (DEBUG_WEB) Serial.println(readString); //print to serial monitor for debuging

	     		printP(client, HEADER1);
	     		client.println();
/*	     		printP(client, HEADER2);
	     		client.println("<BODY>");
				client.println("<H1>Aynur's Chicken Coop</H1>");
				client.println("<hr />");
				client.println("<br />");
				client.println("<H3>Current Status</H3>");
				client.println("<br />");
				client.println("<H6>");
				client.println("<p>Eggs in Basket : 3</p>");
				client.println("<p>Coop Door : Open</p>");
				client.println("<p>Temperature : 5 C</p>");
				client.println("<p>Humidity : 60%</p>");
				client.println("<p>Fan : Off</p>");
				client.println("<p>Red Light : On</p>");
				client.println("<p>Water Heater : Off</p>");
				for (int d = 0 ; d<DEVICE_COUNT ;  d++ ) {
					client.println(devices[d].getName());
					client.println("<br />");
					for (int i = 0 ;i < MAX_NUMBER_OF_VALUES && devices[d].getValueTypebyInd(i)!=NO_VALUE ;  i++ ) {
						printVstr(client, varNames[devices[d].getValueTypebyInd(i)]);
						printP(client, TXTCOLON);
						printVstr(client, devices[d].getValuebyInd(i));
						client.println("<br />");
					}
					client.println("<br />");
				}
				client.println("</H6>");
				client.println("<br />");
				client.println("<br />");
				client.println("<a href=\"/d/1/19\"\">Open Door</a>");
				client.println("<a href=\"/d/1/20\"\">Close Door</a><br />");
				client.println("<br />");
				client.println("<br />");
				client.println("<a href=\"http://vlosite\">VloHome</a>");
				client.println("<a href=\"http://vlosite\">Chicks Cam-1</a>");
				client.println("<a href=\"http://vlosite\">Chicks Cam-2</a>");
				client.println("<br />");
				client.println("</BODY>");
				*/
				client.println("</HTML>");


	           delay(1);
	           //stopping client
	           client.stop();
	           //controls the Arduino if you press the buttons

	           /* get the first token */

	           char * token = strtok(readString, "/");

	           /* walk through other tokens */
        	   int deviceID = 0;
        	   int commandID = 0;
    		   int commandvalue = 0;
        	   token = strtok(NULL, "/"); // d
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
        	   devices[2].commandHandler(deviceID, commandID, commandvalue);

	            //clearing string for next read
	            cptr = 0;
	            readString[cptr] = '\0';

	         }
	       }
	    }
	}
}

void postMessage(byte deviceidx) {
	EthernetClient client;

	if (client.connect(vlosite, 80)) {
	if (DEBUG_WEB) Serial.println("New connection");

		int len = 0;

		printP(client, TXTPOST);
		len = printResponse(client, deviceidx, true);
		client.println(len);
		if (DEBUG_WEB) Serial.println(len);
		client.println();
		if (DEBUG_WEB) Serial.println();
		printResponse(client, deviceidx);
		client.println();

		delay (3);
		while (client.available()) {
			char c = client.read();
			if (DEBUG_WEB) Serial.print(c);
		}

	} else {
		if (DEBUG_WEB) Serial.println("Failed to connect-1");
	}

	delay(1);
	if (DEBUG_WEB) Serial.println("disconnecting.");
	client.stop();
//if (retry==0) Reset_AVR();
}

