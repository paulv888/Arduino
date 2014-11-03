// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section
#ifndef WEB_H_
#define WEB_H_
#include "Arduino.h"
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <SPI.h>
#include "avr/pgmspace.h" // new include
//add your includes for the project command here
#include "configuration.h"
#include "Utils.h"
#include "device.h"
#include "deviceaction.h"
#include "coop.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include "MemoryFree.h"
#include "InternalTemp.h"

#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}

// declare a static string
#define P(name) static const prog_char name[] PROGMEM



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
void postMessage1();
extern Device devices[];
byte findDevice(uint16_t findwhat);

//Do not add code below this line
#endif /* WEB_H_ */
