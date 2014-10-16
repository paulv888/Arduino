// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section
#ifndef rf_H_
#define rf_H_
#include "Arduino.h"
//add your includes for the project command here
#include "configuration.h"
#include "share_commands.h"
#include "RF24Network.h"
#include "RF24.h"
#include "command.h"
#include <avr/io.h>

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif
// Structure of our command

//add your function definitions for the project test1 here
void setupRF();
bool updateRF(bool postmessages);
void sendMessageRF(byte deviceidx, bool wait_response);
extern byte findDevice(uint16_t findwhat, byte bywhat);

//Do not add code below this line
#endif /* rf_H_ */
