// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section
#ifndef ARDBrigde_H_
#define ARDBrigde_H_
#include "Arduino.h"
#include <Ethernet.h>
#include <SPI.h>
//add your includes for the project test here
#include "configuration.h"
#include "rf.h"
#include "Web.h"
#include "device.h"
#include "postclient.h"
#include "Utils/Utils.h"
#include "Utils/MemoryFree.h"

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project test here
int findDevice(uint16_t node_address, int bywhat) ;



//Do not add code below this line
#endif /* test_H_ */
