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

static 	EthernetServer server(80);      //server port
String readString;

void setupWeb(){
	// initialize the Ethernet adapter
	Ethernet.begin(mac, ip, dns, gw, sn );
	server.begin();
	if (DEBUG_MAIN) Serial.print("Coop server is at ");
	if (DEBUG_MAIN) Serial.println(Ethernet.localIP());
}

void updateWeb(){
	  EthernetClient client = server.available();
	  if (client) {
	    while (client.connected()) {
	      if (client.available()) {
	        char c = client.read();

	        //read char by char HTTP request
	        if (readString.length() < 100) {
	          //store characters to string
	          readString += c;
	          //Serial.print(c);
	         }

	         //if HTTP request has ended
	         if (c == '\n') {
	           Serial.println(readString); //print to serial monitor for debuging

	           client.println("HTTP/1.1 200 OK"); //send new page
	           client.println("Content-Type: text/html");
	           client.println();
	           client.println("<HTML>");
	           client.println("<HEAD>");
	           client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
	           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
	           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
	           client.println("<TITLE>Aynur's Chicken Coop</TITLE>");
	           client.println("</HEAD>");
	           client.println("<BODY>");
	           client.println("<H1>Aynur's Chicken Coop</H1>");
	           client.println("<hr />");
	           client.println("<br />");
	           client.println("<H2>Current Status</H2>");
	           client.println("<br />");
	           client.println("<H3>");
	           client.println("<p>Egg Basket   : 3</p><H3>");
	           client.println("<p>Temperature  : 5 C</p>");
	           client.println("<p>Humidity     : 60%</p>");
	           client.println("<p>Fan          : Off</p>");
	           client.println("<p>Red Light    : On</p>");
	           client.println("<p>Water Heater : Off</p><H3>");
	           client.println("</H3>");
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
	           client.println("</HTML>");

	           delay(1);
	           //stopping client
	           client.stop();
	           //controls the Arduino if you press the buttons
	           if (readString.indexOf("/d/1/19") >0){
	               digitalWrite(LED_PIN, HIGH);
	           }
	           if (readString.indexOf("/d/1/20") >0){
	               digitalWrite(LED_PIN, LOW);
	           }
	           if (readString.indexOf("?button2on") >0){
	           }
	           if (readString.indexOf("?button2off") >0){
	           }
	            //clearing string for next read
	            readString="";

	         }
	       }
	    }
	}
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

