// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Range_H_
#define Range_H_
#include "Arduino.h"
//add your includes for the project Range here
#include <stdio.h>
#include "configuration.h"
#include "command.h"
#include "Led.h"
#include <Timer.h>
#include <rf.h>
#include "sensors.h"
#include "utils.h"



//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project Range here
void displayError() ;
boolean read_button(int buttonid);
extern int readsensor(int sensorid);
extern float readDHT11() ;



//Do not add code below this line
#endif /* Range_H_ */
