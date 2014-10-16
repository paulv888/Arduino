  /*
 * command.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: pvloon
 */
// Do not remove the include below
#include "command.h"

//int calibrate_dhtvalue;

command_tRF packCommandRF(uint16_t to, command_tRF command) ;
void unpackCommandRF(uint16_t from, command_tRF command) ;


/*
 'date("d-m-Y H:i:s", $totalSeconds) . ".$extraMilliseconds"."
 '2010-04-24 15:29:44.845 send plc A8 bright 10
 '<?xml version="1.0" encoding="UTF-8"?>
 '<X10Command>
 ' <CallerID>web</CallerID><Operation>send</Operation>
 ' <Sender>plc</Sender>
 ' <HouseCode>A</HouseCode>
 ' <Unit>1</Unit>
 ' <Command>_Address</Command>
 ' <CmdData></CmdData>
 ' <GMTTime>2011-01-12 21:33:00:289525</GMTTime>
 '</X10Command>
 $x10[0]->CallerID = "web";							// not RF, Add on bridge, Always "Ard-Bridge" = 9
 $x10[0]->Operation = "send"; 						// not RF, Add on bridge, "send" = 1, "recv" = 2
 $x10[0]->Sender = "plc";							// not RF, Add on bridge, Always "rf" = 1
 $x10[0]->Command = $rowcommands['description'];

 myid	type	type_raw	description
 20	X10	1	Off
 17	X10	1	On
 8	X10	1	StatusOn
 6	X10	1	StatusOff
 5	X10	1	StatusRequest
 ?	ARD	1	Read Sensors
 ?   ARD 1   Read Configuration

 $x10[0]->HouseCode = $rowdevices['code'];			// not RF, Add on bridge " Always "Y", Arduino Bridge Devices
 $x10[0]->Unit = $rowdevices['unit'];				// this_node, would not support routing/leaves
 $x10[0]->CmdData = $value;
 $x10[0]->GMTTime = udate("Y-m-d H:i:s:u");			// not RF, Add on bridge

 */


void handle_commandRF(uint16_t from, command_tRF command) {

	int i;
	switch (command.command) {
	case COMMAND_ON:
	case COMMAND_STATUSON:
		if ((i = findDevice(from, FIND_BY_NODE_ADDRESS)) >= 0) {
			devices[i].setStatus(true);
			unpackCommandRF(from, command);
			if (DEBUG_COMMAND) Serial.print("DevID: ");
			if (DEBUG_COMMAND) Serial.println(devices[i].getDeviceid());
		}
		// XXX send to server
		break;
	case COMMAND_OFF:
	case COMMAND_STATUSOFF:
		if ((i = findDevice(from, FIND_BY_NODE_ADDRESS)) >= 0) {
			devices[i].setStatus(false);
			unpackCommandRF(from, command);
			if (DEBUG_COMMAND) Serial.print("DevID: ");
			if (DEBUG_COMMAND) Serial.println(devices[i].getDeviceid());
		}
		// XXX send to server
		break;
	}
}

command_tRF packCommandRF(uint16_t to, command_tRF command) {


	if (DEBUG_COMMAND) Serial.print("Status: ");
	if (DEBUG_COMMAND) Serial.println((devices[to].getStatus()) ? "On" : "Off");
	for (int sensorid = 0; sensorid <= 6; sensorid++) {
		command.sensorTemp[sensorid] = (unsigned char)devices[to].sensorTemp[sensorid] ;
		//if (DEBUG_COMMAND) printf("T%u ", sensorid );
		//if (DEBUG_COMMAND) printf(" %u ", command.sensorTemp[sensorid] );
	}
	//command.minTemp = calibrate_dhtvalue;
	//if (DEBUG_COMMAND) printf(" min %u", command.minTemp);
	//command.maxTemp = SENSOR_MAX_TEMP;
	//if (DEBUG_COMMAND) printf(" max %u", command.maxTemp);
	command.switches = 0;
	for (int sensorid = 0; sensorid <= 5; sensorid++) {
		command.switches |= devices[to].switchValue[sensorid] << sensorid;
	}
	//if (DEBUG_COMMAND) printf(" switches %X\n", command.switches);
	return command;
}

void unpackCommandRF(uint16_t from, command_tRF command) {

	int i;
	if ((i = findDevice(from, FIND_BY_NODE_ADDRESS)) < 0) return ;

	//if (DEBUG_COMMAND) printf("\n");
	if (DEBUG_COMMAND) Serial.print("Status: ");
	if (DEBUG_COMMAND) Serial.println((devices[i].getStatus()) ? "On" : "Off");
	//if (DEBUG_COMMAND) printf("mTemp %u \n", command.minTemp);
	//if (DEBUG_COMMAND) printf("maxTemp %u \n", command.maxTemp);
	for (int sensorid = 0; sensorid <= 5; sensorid++) {
		devices[i].sensorTemp[sensorid] = map(command.sensorTemp[sensorid],0,255,command.minTemp,command.maxTemp)  ;
		//if (DEBUG_COMMAND) printf("T%u: ",sensorid );
		//if (DEBUG_COMMAND) printf("%u ", devices[i].sensorTemp[sensorid]);
	}
	devices[i].sensorTemp[6] = command.sensorTemp[6];
	//if (DEBUG_COMMAND) printf("\n");
	for (int sensorid = 0; sensorid <= 5; sensorid++) {
		devices[i].switchValue[sensorid] = (command.switches & (1 << sensorid)) > 0;
		//if (DEBUG_COMMAND) printf("S%u: ",sensorid );
		//if (DEBUG_COMMAND) printf("%u ",devices[i].switchValue[sensorid]);
	}
	//if (DEBUG_COMMAND) printf("\n");
	return;
}


