/*
 * Menu.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: pvloon
 */
#include "Menu.h"

int keypad_value = 0;
Timer timer;

char* fc[]={"F", "C"};
byte menuTimeOut;
static int timerMenuTimeout = -1;

#define MAIN_MENU_ITEMS 5
char* menu[]={"1. Smoker temp  ",
			  "2. Meat 1 temp  ",
			  "3. Meat 2 temp  ",
			  "4. Smoke density",
			  "5. F/C (%s)      ",
			  "                "};

void waitBtnRelease()
{
    while( analogRead(KEYPAD_PIN) > 200){
       DEBUGPRINT("Wait Release ");
       DEBUGPRINT_LF(keypad_value);
       delay(100);
      } 
}

char readKeypad()
{
  /* Keypad button analog Value
  no button pressed 0
  select/right  582
  left          679
  down          816
  up           1022
  right   xxx
  */
  keypad_value = analogRead(KEYPAD_PIN);
  //DEBUGPRINT("Key Value ");
  //DEBUGPRINT_LF(keypad_value);
  
  if(keypad_value > 900) {
	timer.reset(timerMenuTimeout);
    return 'U';
  } else if(keypad_value > 700) {
	timer.reset(timerMenuTimeout);
    return 'D';
  } else if(keypad_value > 630) {
	timer.reset(timerMenuTimeout);
    return 'L';
  } else if(keypad_value > 400) {
	timer.reset(timerMenuTimeout);
    return 'R';
  } else 
    return 'N';
}

int menuValue(char* text, int param, int min, int max)
{  
	char btn_push;
    char buffer [16];
	lcd.clear();
    do {
  		lcd.setCursor(0,0);
  		sprintf(buffer, "%s: %3d", text, param);
  		lcd.print(buffer);
		

		btn_push = readKeypad();
	  	//waitBtnRelease();

		if(btn_push == 'U') {
			param++;
			if (param > max) param = min;
		} else if(btn_push == 'D') {
			param--;
			if (param < min) param = max;
		} else if(btn_push == 'R') {
			btn_push = 'L';
		}

		delay(100);
    } while (btn_push != 'L');
  	return param;
}


void MenuC()
{  
	char btn_push;
    byte cMenuPage = EEPROMReadInt(PARAMS(BINARY_V, CELCIUS));
    lcd.clear();
    do {
		lcd.setCursor(0,0);
		switch (cMenuPage)
		{
			case 0:
			  lcd.print("Fahrenheit");
			  break;
			case 1:
			  lcd.print("Celcius   ");
			  break;
		}

		btn_push = readKeypad();
	  	waitBtnRelease();

		if(btn_push == 'U' || btn_push == 'D')
		{
			cMenuPage = 1 - cMenuPage;
		} else if(btn_push == 'R') {
		  	EEPROMWriteInt(PARAMS(BINARY_V, CELCIUS), cMenuPage);
			btn_push = 'L';
		}

		delay(100);
    } while (btn_push != 'L');
}

void menuTimeout(byte dummy) {
	menuTimeOut = true;
}

void doMenu()
{
	timer.stop(timerMenuTimeout);
	char btn_push;
	int mainMenuPage = 0;
    lcd.clear();
   	waitBtnRelease();
	int mval;
    char buffer [16];
	menuTimeOut = false;
	byte dummy;
	timerMenuTimeout = timer.after(8000, menuTimeout, dummy);	// Menu Timeout timer 
	
    do {
		lcd.setCursor(0,0);
		switch (mainMenuPage)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				lcd.print(menu[mainMenuPage]);
				lcd.setCursor(0,1);
				lcd.print(menu[mainMenuPage+1]);
				break;
			case 4:
				sprintf(buffer, menu[mainMenuPage], fc[EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))]);
				lcd.print(buffer);
				lcd.setCursor(0,1);
				lcd.print(menu[mainMenuPage+1]);
			  break;
		}
		lcd.setCursor(15,0);
		
		btn_push = readKeypad();
      	waitBtnRelease();
    
		if(btn_push == 'U'){
			mainMenuPage--;
			if(mainMenuPage < 0)
				mainMenuPage = MAIN_MENU_ITEMS-1;    
		} else if(btn_push == 'D') {
			mainMenuPage++;
			if(mainMenuPage > MAIN_MENU_ITEMS)
			mainMenuPage = 0;
		}

		if(btn_push == 'R') {  //enter selected menu
			switch (mainMenuPage)
			{
				case 0:
					mval = menuValue("Smoker", (int)toFahrenheit(EEPROMReadInt(PARAMS(ANALOG_V, SMOKER)), EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), 0, 300);
					EEPROMWriteInt(PARAMS(ANALOG_V,SMOKER), (int)toCelcius(mval, EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))));
					break;
				case 1:
					mval = menuValue("Meat 1", (int)toFahrenheit(EEPROMReadInt(PARAMS(ANALOG_V, MEAT1)), EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), 0, 200);
					EEPROMWriteInt(PARAMS(ANALOG_V,MEAT1), (int)toCelcius(mval, EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))));
					break;
				case 2:
					mval = menuValue("Meat 2", (int)toFahrenheit(EEPROMReadInt(PARAMS(ANALOG_V, MEAT2)), EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), 0, 200);
					EEPROMWriteInt(PARAMS(ANALOG_V,MEAT2), (int)toCelcius(mval, EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))));
					break;
				case 3:
					mval = menuValue("Smoke", EEPROMReadInt(PARAMS(ANALOG_V, SMOKE)), 0, 700);
					EEPROMWriteInt(PARAMS(ANALOG_V,SMOKE), mval);
					break;
				case 4:
					MenuC();
				break;
			}
      }
	  
      timer.update();
      delay(100);
    } while (btn_push != 'L' && !menuTimeOut);
    
}
