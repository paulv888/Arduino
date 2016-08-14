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
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Timer.h"

Timer::Timer() {
}

int Timer::every(const long period, void (*callback)(const byte), const byte parameter, const int repeatCount) {
	int i = findFreeEventIndex();
	if (i == -1) {
		if (DEBUG_TIMER) Serial.println("***TERR");
		return -1;
	}

	if (DEBUG_TIMER) Serial.print("Tint: ");
	if (DEBUG_TIMER) Serial.print(i);
	if (DEBUG_TIMER) Serial.print(" perd ");
	if (DEBUG_TIMER) Serial.println(period);

	_events[i].eventType = EVENT_EVERY;
	_events[i].period = period;
	_events[i].repeatCount = repeatCount;
	_events[i].callback = callback;
	_events[i].parameter = parameter;
	_events[i].lastEventTime = millis();
	_events[i].count = 0;
	return i;
}

int Timer::every(const long period, void (*callback)(const byte), const byte parameter) {
	return every(period, callback, parameter, -1); //  means forever
}

int Timer::every(const long period, void (*callback)(const byte)) {
	return every(period, callback, -1, -1); // means no parameter, - means forever
}

int Timer::after(const long period, void (*callback)(const byte), const byte parameter) {
	return every(period, callback, parameter, 2);					// **** 1 off added 1 //
}

int Timer::oscillate(const byte pin, const byte startingValue, const long period, const byte dutyCycle, const int repeatCount) {
	int i = findFreeEventIndex();
	if (i == -1) return -1;

	_events[i].eventType = EVENT_OSCILLATE;
	_events[i].pin = pin;
	_events[i].period = (period * dutyCycle) / 100;
	_events[i].period2 = (period * (100 - dutyCycle)) / 100;
	_events[i].pinState = startingValue;
	digitalWrite(pin, startingValue);
	_events[i].repeatCount = repeatCount * 2; // full cycles not transitions
	_events[i].lastEventTime = millis();
	_events[i].count = 0;

	if (DEBUG_TIMER) Serial.print("Tosc: ");
	if (DEBUG_TIMER) Serial.print(i);
	if (DEBUG_TIMER) Serial.print(" perd ");
	if (DEBUG_TIMER) Serial.print(_events[i].period);
	if (DEBUG_TIMER) Serial.print(" perd2: ");
	if (DEBUG_TIMER) Serial.println(_events[i].period2);

	return i;
}

int Timer::oscillate(const byte pin, const byte startingValue, const long period, const byte dutyCycle) {
	return oscillate(pin, startingValue, period, dutyCycle, -1); // forever
}

void Timer::pulse(const byte pin, const byte startingValue, const long period) {
	oscillate(pin, startingValue, period, 100, 1); // once
}

int Timer::stop(const int id) {
	if (id>=0) _events[id].eventType = EVENT_NONE;
	return -1;
}

void Timer::reset(const int id) {
	if (id>=0) _events[id].lastEventTime = millis();
}

void Timer::update() {
	for (int i = 0; i < MAX_NUMBER_OF_EVENTS; i++) {
		if (_events[i].eventType != EVENT_NONE) {
			_events[i].update();
		}
	}
}

int Timer::findFreeEventIndex() {
	for (int i = 0; i < MAX_NUMBER_OF_EVENTS; i++) {
		if (_events[i].eventType == EVENT_NONE) {
			return i;
		}
	}
	return -1;
}

