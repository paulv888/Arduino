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
#ifndef COMMAND_H_
#define COMMAND_H_
#include "Arduino.h"

//add your includes for the project command here
#include "configuration.h"
#include "rf.h"
#include "device.h"
#include "Utils/Utils.h"
#include "Utils/MemoryFree.h"


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
void handle_commandRF(uint16_t from, command_tRF command);
extern void sendMessageRF(uint16_t, command_tRF);
extern int findDevice(uint16_t findwhat, int bywhat) ;
void sendWeb(uint16_t from, command_tRF command) ;
extern DEVICE devices[];

//Do not add code below this line
#endif /* COMMAND_H_ */
