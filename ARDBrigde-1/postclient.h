// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section
#ifndef POSTCLIENT_H_
#define POSTCLIENT_H_
#include "Arduino.h"
#include <stdio.h>
#include <SPI.h>
#include <Ethernet.h>
//add your includes for the project command here
#include "configuration.h"
#include "Utils/Utils.h"


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
void postMessage();
void workingGetMessage();

//Do not add code below this line
#endif /* POSTCLIENT_H_ */



