// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section
#ifndef Coop_H_
#define Coop_H_
#include <SPI.h>
#include <Ethernet.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <dht.h>
#include <Event.h>
#include <Timer.h>
#include <avr/wdt.h>  
#include "Configuration.h"
#include "Devicehandler.h"
#include "Door.h"

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

