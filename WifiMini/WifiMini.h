// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section
#ifndef WifiMini_H_
#define WifiMini_H_
//#include <SPI.h>
//#include <Ethernet.h>
//#include <dht.h>

#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <Event.h>
#include <Timer.h>
#include <avr/wdt.h>  
#include "Configuration.h"
#include "Devicehandler.h"

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
#endif /* WifiMini_H_ */

