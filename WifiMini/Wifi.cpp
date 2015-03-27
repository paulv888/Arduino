/*
 * Wifi.cpp
 *
 *  Created on: March 2, 2015
 *      Author: pvloon
 */
 /* ====== ESP8266 Toy Demo ============
* A simple web server
* - serve HTML page from flash memory
* - read and display analog pin values
* - set RGB led color
* ====================================
*
* Change SSID and PASS to match your WiFi settings.
* The IP address is displayed to debug Serial
* upon successful connection.
*
* Written by Ray Wang @ Rayshobby LLC
* http://rayshobby.net/?p=10156
*/
#include "Wifi.h"

// Ethernet buffer
char buffer[BUFFER_SIZE];

// If using software Serial for debugging
// use the definitions below

// By default we are looking for OK\r\n
char OKrn[] = "OK\r\n";

// wait for ESP8266 to respond until string term is found
// or timeout has reached
// return true if term is found

bool wait_for_esp_response(int timeout=1000, char* term=OKrn) {
	unsigned long t=millis();
	bool found=false;
	int i=0;
	int len=strlen(term);
	// wait for at most timeout milliseconds
	// or if term is found
	while(millis()<t+timeout) {
		if(esp.available()) {
			buffer[i++]=esp.read();
			if(i>=len) {
				if(strncmp(buffer+i-len, term, len)==0) {
					found=true;
					break;
				}
			}
		}
	}
	buffer[i]=0;
	return found;
}

byte send_at(String comm, int timeout=1000) {
	esp.println(comm);
	dbg.println(comm);
	return wait_for_esp_response(timeout);
}

// read data from ESP8266 until \r\n is found
// non-blocking
bool read_till_eol() {
	static int i=0;
	if(esp.available()) {
		buffer[i++]=esp.read();
		if(i==BUFFER_SIZE) i=0;
		if(i>1 && buffer[i-2]==13 && buffer[i-1]==10) {
			buffer[i]=0;
			i=0;
			return true;
		}
	}
return false;
}

// extract the action string of get command
void get_comm(char* src, char* dst, int size) {
	int i;
	for(i=0;i<size;i++) {
		dst[i]=src[i];
		if(dst[i]==' ') break;
	}
	dst[i]=0;
}
void Wifi::begin(const int _deviceID, const int _deviceIdx) {
/*!
*		Initializes a device. 
*/
	esp.begin(ESP_BAUD_RATE);
	dbg.begin(DBG_BAUD_RATE);
	// print device IP address


	// query esp
	send_at("AT");
	// set mode 1 (client)
	send_at("AT+CWMODE=1");
	// reset WiFi module
	send_at("AT+RST", 3000);
	delay(3000);
	// join AP
	// this may take a while, so set 5 seconds timeout
	String comm="AT+CWJAP=\"";
	comm+=SSID;
	comm+="\",\"";
	comm+=PASS;
	comm+="\"";
	send_at(comm, 5000);
	// start server
	send_at("AT+CIPMUX=1");
	comm="AT+CIPSERVER=1,"; // turn on TCP service
	comm+=PORT;
	send_at(comm);

	dbg.print(F("ip addr:"));
	send_at("AT+CIFSR");
	dbg.print(buffer);
}

bool Wifi::send_data(String data, int ch_id, bool close=false) {
	bool success = false;
	esp.print("AT+CIPSEND=");
	esp.print(ch_id);
	esp.print(",");
	esp.println(data.length());
	if(wait_for_esp_response(2000, "> ")) {
		esp.print(data);
		if(wait_for_esp_response(5000)) {
			success = true;
		}
	}
	if(close) {
		esp.print("AT+CIPCLOSE=");
		esp.println(ch_id);
		wait_for_esp_response();
	}
	return success;
}


