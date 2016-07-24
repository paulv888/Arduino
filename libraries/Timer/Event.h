
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
// For Arduino 1.0 and earlier

#ifndef Event_h
#define Event_h

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <inttypes.h>

#define EVENT_NONE 0
#define EVENT_EVERY 1
#define EVENT_OSCILLATE 2



class Event
{

public:
  Event(); 
  void update();
  byte eventType;
  long period;
  long period2;
  int repeatCount;
  byte parameter;
  byte pin;
  byte pinState;
  void (*callback)(const byte);
  unsigned long lastEventTime;
  int count;
};

#endif
