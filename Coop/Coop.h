// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef ARDBrigde_H_
#define ARDBrigde_H_
#include "Arduino.h"
#include "configuration.h"
#include "share_commands.h"
#include "device.h"
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include "Web.h"
#include "Led.h"
#include <Timer.h>


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project ARDBrigde here
byte findDevice(uint16_t findwhat, byte bywhat);
void displayError() ;
boolean read_button(int buttonid);
extern int readsensor(int sensorid);
extern float readDHT11() ;




//Do not add code below this line
#endif /* ARDBrigde_H_ */
