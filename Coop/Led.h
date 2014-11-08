// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Led_H_
#define Led_H_
#include "Arduino.h"
//add your includes for the project Led here
#include "share_commands.h"
#include "Configuration.h"
#include <Event.h>
#include <Timer.h>
#include "Web.h"

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project Led here
void showStatus(byte Error, byte Idx);
extern Timer timer;

//extern Device mdevices[];


#endif /* Led_H_ */
