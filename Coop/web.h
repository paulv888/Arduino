// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section
#ifndef WEB_H_
#define WEB_H_
#include "Arduino.h"
#include <Ethernet.h>
#include <SPI.h>
#include "avr/pgmspace.h" // new include
//add your includes for the project command here
#include "configuration.h"
#include "WebServer.h"
#include "Utils.h"
#include "device.h"
#include "MemoryFree.h"
#include <avr/io.h>
#include <avr/wdt.h>

#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}



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
#define WEBDUINO_FAVICON_DATA ""
#define WEBDUINO_FAIL_MESSAGE ""
#define VERSION_STRING "0.1"


void setupWeb();
void updateWeb();
void postMessage( byte deviceidx, byte cmdType ) ;
extern DEVICE devices[];


//Do not add code below this line
#endif /* WEB_H_ */
