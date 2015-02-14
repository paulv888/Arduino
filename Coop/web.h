
// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section
#ifndef WEB_H_
#define WEB_H_
#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
#include <avr/pgmspace.h>
//add your includes for the project command here
#include "Configuration.h"
#include "Device.h"
#include "MemoryFree.h"
#include "Utils.h"

#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}

// declare a static string
#define P(name) static const char name[] PROGMEM

// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

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
void setupWeb();
void updateWeb();
void postMessage( byte deviceidx);
//extern Device mdevices[];
extern byte * heapptr;
extern byte * stackptr;

byte findDevice(uint16_t findwhat);

//Do not add code below this line
#endif /* WEB_H_ */

