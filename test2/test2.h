// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef test2_H_
#define test2_H_
#include "Arduino.h"
//add your includes for the project test2 here
// declare a static string
 #include <avr/pgmspace.h> // new include

#define P(name)   static const prog_char name[] PROGMEM

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
#endif /* test2_H_ */
