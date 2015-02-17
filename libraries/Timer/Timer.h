/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 Code by Simon Monk
 http://www.simonmonk.org
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Timer_h
#define Timer_h

#include <inttypes.h>
#include "Event.h"

//#define DEBUG_TIMER true
#ifndef DEBUG_TIMER
#define DEBUG_TIMER false
#endif

#define MAX_NUMBER_OF_EVENTS 10

class Timer {

public:
	Timer();

	int every(const long period, void (*callback)(const byte));
	int every(const long period, void (*callback)(const byte), const byte parameter);
	int every(const long period, void (*callback)(const byte), const byte parameter, const int repeatCount);
	int after(const long duration, void (*callback)(const byte), const byte parameter);
	int oscillate(const byte pin, const byte startingValue, const long period, const byte dutyCycle);
	int oscillate(const byte pin, const byte startingValue, const long period, const byte dutyCycle, const int repeatCount);
	void pulse(const byte pin, const byte startingValue, const long period);
	int stop(const int id);
	void update();
	void updatePeriod(const int id, const long period);

protected:
	Event _events[MAX_NUMBER_OF_EVENTS];
	int findFreeEventIndex();

};

#endif
