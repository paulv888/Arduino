// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section
#ifndef Coop_H_
#define Coop_H_
#include "Arduino.h"
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <Timer.h>
#include "Configuration.h"
#include "Device.h"
#include "Deviceaction.h"
#include "Door.h"
#include "Thermo.h"

//end of add your includes here

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif


//Do not add code below this line
#endif /* Coop_H_ */
