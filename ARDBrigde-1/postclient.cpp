/*
 * postclient.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: pvloon
 */

#include "postclient.h"


void postMessage() {

	byte subnet[] = { 255, 255, 255, 0 };
	byte gateway[] = { 192, 168, 0, 254 };
	byte server[] = { 192, 168, 2, 110 }; // Google IP

	EthernetClient client;

	String data;
	data += "";
	data += "entry.0.single=";
	data += analogRead(A0);
	data += "&entry.2.single=";
	data += analogRead(A1);
	data += "&submit=Submit";

	if (client.connect(server,80)) {
		Serial.println("connected");

		client.print("POST //cronjobs/str.php?XDEBUG_SESSION_START=ECLIPSE_DBGP&KEY=13467253808211");
		client.println(" HTTP/1.1");
		client.println("Host: spreadsheets.google.com");
		client.println("Content-Type: application/x-www-form-urlencoded");
		client.println("Connection: close");
		client.print("Content-Length: ");
		client.println(data.length());
		client.println();
		client.print(data);
		client.println();

		client.print("POST //cronjobs/str.php?XDEBUG_SESSION_START=ECLIPSE_DBGP&KEY=13467253808211");
		client.println(" HTTP/1.1");
		Serial.println("Host: spreadsheets.google.com");
		Serial.println("Content-Type: application/x-www-form-urlencoded");
		Serial.println("Connection: close");
		Serial.print("Content-Length: ");
		Serial.println(data.length());
		Serial.println();
		Serial.print(data);
		Serial.println();

	}
	delay(1000);
	if (!client.connected()) {
		Serial.println();
		Serial.println("disconnecting.");
		client.stop();
	}
}


