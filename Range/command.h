/*
 * command.h
 *
 *  Created on: Sep 1, 2012
 *      Author: pvloon
 */
// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef command_H_
#define command_H_
#include "Arduino.h"
//add your includes for the project command here
#include "share_commands.h"
#include "configuration.h"
#include "MemoryFree.h"
#include "sensors.h"

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project command here
void handle_commandIn(uint16_t from, payLoad_Simple &command);
void packCommandRF(payLoad_Value_Pair &command);
void packCommandRF(payLoad_DeviceStatus &command);

extern byte switchValue[6][2];					// Current switch Status
extern int sensorValue[7][2];  				// Current readings   6 holds DHT11 Temp
extern unsigned long sensorAge[7];  				// Current readings   6 holds DHT11 Temp
extern byte currentStatus;					// Current Range Status
extern int calibrate_dhtvalue;
extern unsigned long rfTransmitPackets;
extern unsigned long rfReceivePackets;
extern unsigned long rfSendErrors;

//Do not add code below this line
#endif /* command_H_ */
