/*
 * Web.cpp
 *
 *  Created on: Sep 2, 2012
 *      Author: pvloon
 */
#include "Web.h"

// ROM-based messages used by the application
// These are needed to avoid having the strings use up our limited
//    amount of RAM.
P(Page_start) = "<html><head><title>ARD_Bridge " VERSION_STRING "</title></head><body>\n";
P(Page_end) = "</body></html>";
P(OpenTag) = "<";
P(CloseTag) = "</";
P(Tail_end) = "\n";
P(Status) = "Status>";
P(HeatSensors) = "HeatSensors>";
P(Temperature) = "Temperature>";
P(Switches) = "Switches>";
P(Switch) = "Switch>";
P(Uptime) = "Uptime>";
P(sFreeMemory) = "FreeMemory>";
P(sRFLastSuccesfull) = "RFLastSuccesfull>";
P(sRFSendErrors) = "RFSendErrors>";
P(sResponse) = "Response>";
P(LastMessageAge) = "LastMessageAge>";

/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
#define PREFIX ""
WebServer webserver(PREFIX, 80);

void printResponse(WebServer &server, int i) {

	server.printP(OpenTag);
	server.printP(Status);
	server.print((devices[i].getStatus() > 0) ? "On" : "Off");
	server.printP(CloseTag);
	server.printP(Status);
	server.printP(Tail_end);
	server.printP(OpenTag);
	server.printP(LastMessageAge);
	server.print((int)(devices[i].ageLastMessage()/1000));
	server.printP(CloseTag);
	server.printP(LastMessageAge);
	server.printP(Tail_end);
	server.printP(OpenTag);
	server.printP(HeatSensors);
	server.printP(Tail_end);
	for (int sensorid = 0; sensorid <= 5; sensorid++) {
		server.printP(OpenTag);
		server.printP(Temperature);
		server.print(devices[i].sensorTemp[sensorid]);
		server.printP(CloseTag);
		server.printP(Temperature);
		server.printP(Tail_end);
	}
	server.printP(OpenTag);
	server.printP(Temperature);
	server.print(devices[i].sensorTemp[6]);
	server.printP(CloseTag);
	server.printP(Temperature);
	server.printP(Tail_end);
	server.printP(CloseTag);
	server.printP(HeatSensors);
	server.printP(Tail_end);
	server.printP(OpenTag);
	server.printP(Switches);
	for (int sensorid = 0; sensorid <= 5; sensorid++) {
		server.printP(OpenTag);
		server.printP(Switch);
		server.print((devices[i].switchValue[sensorid] > 0 ? "On" : "Off"));
		server.printP(CloseTag);
		server.printP(Switch);
		server.printP(Tail_end);
	}
	server.printP(CloseTag);
	server.printP(Switches);
	server.printP(Tail_end);
}

void printMyStatus(WebServer &server) {

	server.printP(OpenTag);
	server.printP(Status);
	server.print("StatusOn");
	server.printP(CloseTag);
	server.printP(Status);
	server.printP(Tail_end);
	server.printP(OpenTag);
	server.printP(Uptime);
//	server.print(time(millisRollover()+millis()/1000));
	server.print((int)(millis()/1000));
	server.printP(CloseTag);
	server.printP(Uptime);
	server.printP(Tail_end);
	server.printP(OpenTag);
	server.printP(sFreeMemory);
	server.print(freeMemory());
	server.printP(CloseTag);
	server.printP(sFreeMemory);
	server.printP(Tail_end);
	server.printP(OpenTag);
	server.printP(sRFLastSuccesfull);
//	server.print(time(rfLastSuccesful/1000));
	server.print((int)(rfLastSuccesful/1000));
	server.printP(CloseTag);
	server.printP(sRFLastSuccesfull);
	server.printP(Tail_end);
	server.printP(OpenTag);
	server.printP(sRFSendErrors);
	server.print(rfSendErrors);
	server.printP(CloseTag);
	server.printP(sRFSendErrors);
	return;
}

/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
#define NAMELEN 32
#define VALUELEN 32

void processCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
	URLPARAM_RESULT rc;
	char name[NAMELEN];
	char value[VALUELEN];
	int commandid = 0, deviceid = 0;

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
					if (strcmp(name,"cmdid") == 0) commandid = atoi(value);
					if (strcmp(name,"devid") == 0) deviceid = atoi(value);
				}
			}
			// Handle command
			//result = handleCommand(cmd, commandcounter, response);

			if (DEBUG_WEB) Serial.print("cmdid: ");
			if (DEBUG_WEB) Serial.print(commandid);
			if (DEBUG_WEB) Serial.print(" devid: ");
			if (DEBUG_WEB) Serial.println(deviceid);
			if (commandid > 0 && deviceid > 0) {
				switch (commandid) {
				case COMMAND_STATUSREQUEST:
					int i;
					i = findDevice(deviceid, FIND_BY_DEVICE_ID);
					if (i >= 0) {
						server.httpSuccess();
						printResponse(server, i);
						return;
					} else if (deviceid == DEVICE_0_ID) { // Oh its me
						server.httpSuccess();
						printMyStatus(server);
						return;
					} else {
						response = 100; //"Unknown device" + deviceid;
					}
					break;
				case COMMAND_ON:
				case COMMAND_STATUSON:
				case COMMAND_OFF:
				case COMMAND_STATUSOFF:
				default:
					response = 101; //"Unknown CommandID";
					break;
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

void my_failCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
	/* this line sends the standard "we're all OK" headers back to the
	 browser */
	server.httpFail();
	server.printP(Page_start);
	server.printP(OpenTag);
	server.printP(sResponse);
	server.print(150, DEC);
	server.printP(CloseTag);
	server.printP(sResponse);
	server.printP(Page_end);

	/* if we're handling a GET or POST, we can output our data here.
	 For a HEAD request, we just stop after outputting headers. */
	if (type == WebServer::HEAD) return;
}

void setupWeb() {

	/* setup our default command that will be run when the user accesses
	 * a page NOT on the server */
	webserver.setFailureCommand(&my_failCmd);

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
