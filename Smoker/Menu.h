/*
 * Utils.h
 *
 *  Created on: Sep 3, 2012
 *      Author: pvloon
 */

#ifndef MENU_H_
#define MENU_H_
#include "Arduino.h"
#include "Configuration.h"
#include "Utils.h"
#include <LiquidCrystal.h>
#include <Timer.h>

#define KEYPAD_PIN A7


void doMenu();
char readKeypad();
extern LiquidCrystal lcd;
#endif /* MENU_H_ */

