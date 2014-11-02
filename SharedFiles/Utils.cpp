/*
 * Utils.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: pvloon
 */
#include "Utils.h"


/* Millis Rollover
 *
 * Example of counting how many times the millis() function has rolled over
 *  by Rob Faludi http://www.faludi.com
 *
 * for the ATMEGA168 with Arduino 0010 the max value is 34359737 or about 9 hours and 32 mintues
 * for more information see: http://www.arduino.cc/en/Reference/Millis
 *
 */


int millisRollover() {
  // get the current millis() value for how long the microcontroller has been running
  //
  // To avoid any possiblity of missing the rollover, we use a boolean toggle that gets flipped
  //   off any time during the first half of the total millis period and
  //   then on during the second half of the total millis period.
  // This would work even if the function were only run once every 4.5 hours, though typically,
  //   the function should be called as frequently as possible to capture the actual moment of rollover.
  // The rollover counter is good for over 35 years of runtime. --Rob Faludi http://rob.faludi.com
  //
  static int numRollovers=0; 					// variable that permanently holds the number of rollovers since startup
  static bool readyToRoll = false; 				// tracks whether we've made it halfway to rollover
  unsigned long now = millis(); 				// the time right now
  unsigned long halfwayMillis = 17179868; 		// this is halfway to the max millis value (17179868)

  if (now > halfwayMillis) { 					// as long as the value is greater than halfway to the max
    readyToRoll = true; 						// you are ready to roll over
  }

  if (readyToRoll == true && now < halfwayMillis) {
    // if we've previously made it to halfway
    // and the current millis() value is now _less_ than the halfway mark
    // then we have rolled over
    numRollovers = numRollovers++; // add one to the count the number of rollovers
    readyToRoll = false; // we're no longer past halfway
  }
  return numRollovers;
}

char* malcpy(const char* s) {
    char* c = (char*)malloc(strlen(s)+1);
    strcpy(c,s);
    return c;
}

