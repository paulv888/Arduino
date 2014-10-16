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
#include "RF24Network.h"
#include "RF24.h"
#include "command.h"


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
void setupRF() ;
void updateRF() ;
//void sendMessageRF(uint16_t sentto, command_tRF command);

//Do not add code below this line
#endif /* rf_H_ */
