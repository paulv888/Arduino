// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef rf_H_
#define rf_H_
#include "Arduino.h"
#include "share_commands.h"
#include "configuration.h"
#include "RF24Network.h"
#include "RF24.h"
#include "command.h"
#include "Led.h"


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project test1 here
void setupRF() ;
void updateRF() ;
bool sendMessageRF(uint16_t sentto, payLoad_Simple &command);
bool sendMessageRF(uint16_t sentto, payLoad_Value_Pair &command);
bool sendMessageRF(uint16_t sentto, payLoad_DeviceStatus &command);

//Do not add code below this line
#endif /* rf_H_ */
