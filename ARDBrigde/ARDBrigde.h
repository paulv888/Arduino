// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef ARDBrigde_H_
#define ARDBrigde_H_
#include "Arduino.h"
#include "configuration.h"
#include "share_commands.h"
#include "device.h"
#include <Ethernet.h>
#include "rf.h"
#include "Web.h"
//#include "Timer.h"
#include "SimpleTimer.h"


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project ARDBrigde here
byte findDevice(uint16_t findwhat, byte bywhat);




//Do not add code below this line
#endif /* ARDBrigde_H_ */
