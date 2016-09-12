/*************************************************************************************************************************
 * Arduino project "ESP Easy" ? Copyright www.esp8266.nu
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * You received a copy of the GNU General Public License along with this program in file 'License.txt'.
 *
 * IDE download    : https://www.arduino.cc/en/Main/Software
 * ESP8266 Package : https://github.com/esp8266/Arduino
 *
 * Source Code     : https://sourceforge.net/projects/espeasy/
 * Support         : http://www.esp8266.nu
 * Discussion      : http://www.esp8266.nu/forum/
 *
 * Additional information about licensing can be found at : http://www.gnu.org/licenses
 **********************************************************************************************************************/
//#include <avr/io.h>
//#include <avr/wdt.h>

// This file is to be loaded onto an Arduino Pro Mini so it will act as a simple IO extender to the ESP module.
// Communication between ESP and Arduino is using the I2C bus, so only two wires needed.
// It best to run the Pro Mini on 3V3, although the 16MHz versions do not officially support this voltage level on this frequency.
// That way, you can skip levelconverters on I2C.
// Arduino Mini Pro uses A4 and A5 for I2C bus. ESP I2C can be configured but they are on GPIO-4 and GPIO-5 by default.

#include "Configuration.h"
#include "Utils.h"
#include "Menu.h"

//
//  Thermocouples
//
#include "max6675.h"
#define MAX_CS1 4
#define MAX_CS2 5
#define MAX_CS3 6
#define MAX_DO  7
#define MAX_CLK 8
MAX6675 thermocouple1(MAX_CLK, MAX_CS1, MAX_DO);
MAX6675 thermocouple2(MAX_CLK, MAX_CS2, MAX_DO);
MAX6675 thermocouple3(MAX_CLK, MAX_CS3, MAX_DO);

//
//  DHT22
//
//#include <DHT.h>
//#define DHTPIN            16        // Pin which is connected to the DHT sensor.
//#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//DHT dht(DHTPIN, DHTTYPE);

//
// Dust GP2Y1010AU0F
//
#define DUST_LED_OUT 15
#define DUST_AN_IN 20
#define DUST_SAMPLING_TIME 25

#include <Wire.h>
#define I2C_MSG_IN_SIZE    4
#define I2C_MSG_OUT_SIZE   4

#define CMD_DIGITAL_WRITE  1
#define CMD_DIGITAL_READ   2
#define CMD_ANALOG_WRITE   3
#define CMD_ANALOG_READ    4


 /*The circuit:
 * LCD RS pin to digital pin 21
 * LCD Enable pin to digital pin 20
 * LCD D4 pin to digital pin 17
 * LCD D5 pin to digital pin 9
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

#define LCD_RS 10
#define LCD_EN 14
#define LCD_D4 17
#define LCD_D5 9
#define LCD_D6 3
#define LCD_D7 2
#define LCD_WIDTH 16
#define LCD_HEIGHT 2

#define NUM_SAMPLES 10


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
char* onoff[]={"_", "*"};
int c = 0;

int tSmoker;
int tSmokersamples[NUM_SAMPLES-1];
int tMeat1;
int tMeat1samples[NUM_SAMPLES-1];
int tMeat2;
int tMeat2samples[NUM_SAMPLES-1];
int dSmoke;
int dSmokesamples[NUM_SAMPLES-1];
byte sSmoker;
byte sMeat1;
byte sMeat2;
byte sSmoke;
int toggle = 0;
byte espReady = 0;

extern char *options_ph[];

volatile uint8_t sendBuffer[I2C_MSG_OUT_SIZE];

void setup()
{
//MCUSR = 0;
//wdt_disable( );
//  wdt_enable(WDTO_8S);
  Wire.begin(0x7f);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  if (DEBUG) Serial.begin(115200);
  delay(10);
  DEBUGPRINT_LF ("Booted...");
  //dht.begin();
  pinMode(DUST_LED_OUT, OUTPUT);
  EEPROMWriteInt(PARAMS(ANALOG_V, SMOKE_SAMPLE_TIME), DUST_SAMPLING_TIME);
  lcd.begin(LCD_WIDTH,LCD_HEIGHT);
  // Print a message to the LCD.
  //wdt_reset(); 
  //readValues();
  waitOnESP();
  lcd.clear();
  lcd.print("System Ready!!! ");
  lcd.blink();
  delay(500);
  pinMode(KEYPAD_PIN, INPUT); // sets analog pin for input 
}


  
// basic readout test, just print the current temp
void loop()
{
	int sample;
    char btn_push = readKeypad();
    if(btn_push != 'N') //enter menu mode
      doMenu();

    if (EEPROMReadInt(PARAMS(BINARY_V, PHASE)) != 0) {
		for(int sample=0; sample < NUM_SAMPLES; sample++) {
			readValues(sample);
		    delay(100);
		}
	}
	avgValues();
	setStates();
    displayValues();
    //delay(0);
}  


void readValues(int sample) {

  if (EEPROMReadInt(PARAMS(ANALOG_V, SMOKER)) > 0) { 
    tSmokersamples[sample] = (int)thermocouple1.readCelsius();
  } else {
    tSmokersamples[sample] = 0;
  }


  if (EEPROMReadInt(PARAMS(ANALOG_V, MEAT1)) > 0) {            //  Enabled
    tMeat1samples[sample] = (int)thermocouple2.readCelsius();
  } else {
    tMeat1samples[sample] = 0;
  }

  if (EEPROMReadInt(PARAMS(ANALOG_V, MEAT2)) > 0) {            //  Enabled
    tMeat2samples[sample] = (int)thermocouple3.readCelsius();
  } else {
    tMeat2samples[sample] = 0;
  }

  if (EEPROMReadInt(PARAMS(ANALOG_V, SMOKE)) > 0) {            //  Switch on if below set point
    digitalWrite(DUST_LED_OUT,LOW); // power on the LED
    delayMicroseconds(EEPROMReadInt(PARAMS(ANALOG_V, SMOKE_SAMPLE_TIME)));
    dSmokesamples[sample] = analogRead(DUST_AN_IN); // read the dust value
    delayMicroseconds(40);
    digitalWrite(DUST_LED_OUT,HIGH); // turn the LED off
  } else {
    dSmokesamples[sample] = 0;
  }
}

void avgValues(){
int tSmokert = 0;
int tMeat1t = 0;
int tMeat2t = 0;
int dSmoket = 0;

	DEBUGPRINT("Sample ");
	DEBUGPRINT("Smoker ");
	DEBUGPRINT("Meat1  ");
	DEBUGPRINT("Meat2  ");
	DEBUGPRINT_LF("Smoke  ");

	for(int sample=0; sample < NUM_SAMPLES; sample++) {
		tSmokert += tSmokersamples[sample];
		tMeat1t += tMeat1samples[sample];
		tMeat2t += tMeat2samples[sample];
		dSmoket += dSmokesamples[sample];
		DEBUGPRINT(sample);
		DEBUGPRINT(" ");
		DEBUGPRINT(tSmokersamples[sample]);
		DEBUGPRINT(" ");
		DEBUGPRINT(tMeat1samples[sample]);
		DEBUGPRINT(" ");
		DEBUGPRINT(tMeat2samples[sample]);
		DEBUGPRINT(" ");
		DEBUGPRINT_LF(dSmokesamples[sample]);
	}
	tSmoker = tSmokert / NUM_SAMPLES;
	tMeat1  = tMeat1t  / NUM_SAMPLES;
	tMeat2  = tMeat2t  / NUM_SAMPLES;
	dSmoket = dSmoket  / NUM_SAMPLES;
	DEBUGPRINT("Avg ");
	DEBUGPRINT(tSmoker);
	DEBUGPRINT(" ");
	DEBUGPRINT(tMeat1);
	DEBUGPRINT(" ");
	DEBUGPRINT(tMeat2);
	DEBUGPRINT(" ");
	DEBUGPRINT_LF(dSmoke);
}
	
void setStates() {

  DEBUGPRINT("Phase ");
  DEBUGPRINT_LF(EEPROMReadInt(PARAMS(BINARY_V, PHASE)));
	
  if (EEPROMReadInt(PARAMS(ANALOG_V, SMOKER)) > 0) { 
    if (tSmoker <= EEPROMReadInt(PARAMS(ANALOG_V, SMOKER))) {            //  Switch on if below set point
      sSmoker = 1;
    } else if (tSmoker >= (EEPROMReadInt(PARAMS(ANALOG_V, SMOKER)) + EEPROMReadInt(PARAMS(ANALOG_V, SMOKER_THRESHOLD)))) {  // Switch off if above threshold
      sSmoker = 0;
    }
  } else {
    sSmoker = 0;
  }
  if (EEPROMReadInt(PARAMS(BINARY_V, PHASE)) == 0 || EEPROMReadInt(PARAMS(BINARY_V, PHASE)) == 3)  {  // Off or Cooldown
    sSmoker = 0;
  }
  DEBUGPRINT("Smoker ");
  DEBUGPRINT(tSmoker);
  DEBUGPRINT(" ");
  DEBUGPRINT_LF(sSmoker);
 

  if (EEPROMReadInt(PARAMS(ANALOG_V, MEAT1)) > 0) {            //  Enabled
    if (tMeat1 <= EEPROMReadInt(PARAMS(ANALOG_V, MEAT1))) {            //  Switch on if below set point
      sMeat1 = 0;
    } else if (tMeat1 >= EEPROMReadInt(PARAMS(ANALOG_V, MEAT1))) {  // Switch off if above threshold
      sMeat1 = 1;
    }
  } else {
    sMeat1 = 0;
  }
  DEBUGPRINT("Meat1  ");
  DEBUGPRINT(tMeat1);
  DEBUGPRINT(" ");
  DEBUGPRINT_LF(sMeat1);

  if (EEPROMReadInt(PARAMS(ANALOG_V, MEAT2)) > 0) {            //  Enabled
    if (tMeat2 <= EEPROMReadInt(PARAMS(ANALOG_V, MEAT2))) {            //  Switch on if below set point
      sMeat2 = 0;
    } else if (tMeat2 >= EEPROMReadInt(PARAMS(ANALOG_V, MEAT2))) {  // Switch off if above threshold
      sMeat2 = 1;
    }
  } else {
    sMeat2 = 0;
  }
  DEBUGPRINT("Meat2  ");
  DEBUGPRINT(tMeat2);
  DEBUGPRINT(" ");
  DEBUGPRINT_LF(sMeat2);


  if (EEPROMReadInt(PARAMS(ANALOG_V, SMOKE)) > 0) {            //  Switch on if below set point
    if (dSmoke <= EEPROMReadInt(PARAMS(ANALOG_V, SMOKE))) {            //  Switch on if below set point
      sSmoke = 0;
    } else if (dSmoke >= (EEPROMReadInt(PARAMS(ANALOG_V, SMOKE)) + EEPROMReadInt(PARAMS(ANALOG_V, SMOKE_THRESHOLD)))) {  // Switch off if above threshold
      sSmoke = 1;
    }
  } else {
    sSmoke = 1;
  }
  DEBUGPRINT("Smoke  ");
  DEBUGPRINT(dSmoke);
  DEBUGPRINT(" ");
  DEBUGPRINT_LF(sSmoke);

}

void displayValues() {
	char line1[] = "                ";
	char line2[] = "                ";
	//lcd.clear();
  
	toggle = 1 - toggle;
	switch (EEPROMReadInt(PARAMS(BINARY_V, PHASE))) {
		case 0 : 	// Off
			toggle = 0;
			sprintf(line2, "%s", options_ph[0]);
			break; 
		case 1 :	// Pre - Heat
			toggle = toggle && (!sSmoker && sSmoke);
			sprintf(line1, "%3d-%3d%s%3d-%3d%s", (int)toFahrenheit(EEPROMReadInt(PARAMS(ANALOG_V, SMOKER)), EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), 
				 (int)toFahrenheit(tSmoker,EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), onoff[sSmoker], EEPROMReadInt(PARAMS(ANALOG_V, SMOKE)), dSmoke, onoff[sSmoke]);
			sprintf(line2, "%s", options_ph[1]);
			break; 
		case 2 :	// Running
			toggle = toggle && (sMeat1 || sMeat2 || !sSmoke);
			sprintf(line1, "%3d-%3d%s%3d-%3d%s", (int)toFahrenheit(EEPROMReadInt(PARAMS(ANALOG_V, SMOKER)), EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), 
				 (int)toFahrenheit(tSmoker,EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), onoff[sSmoker], EEPROMReadInt(PARAMS(ANALOG_V, SMOKE)), dSmoke, onoff[sSmoke]);
			sprintf(line2, "%3d-%3d%s%3d-%3d%s", 
				(int)toFahrenheit(EEPROMReadInt(PARAMS(ANALOG_V, MEAT1)), EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))),
				(int)toFahrenheit(tMeat1, EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), onoff[sMeat1], 
				(int)toFahrenheit(EEPROMReadInt(PARAMS(ANALOG_V, MEAT2)), EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))),
				(int)toFahrenheit(tMeat2, EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), onoff[sMeat2]);
		   break; 
		case 3 :	// Cooldown
			toggle = 0;
			sprintf(line1, "%3d-%3d%s%3d-%3d%s", (int)toFahrenheit(EEPROMReadInt(PARAMS(ANALOG_V, SMOKER)), EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), 
				 (int)toFahrenheit(tSmoker,EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), onoff[sSmoker], EEPROMReadInt(PARAMS(ANALOG_V, SMOKE)), dSmoke, "x");
			sprintf(line2, "%3d-%3d%s%3d-%3d%s", 
				(int)toFahrenheit(EEPROMReadInt(PARAMS(ANALOG_V, MEAT1)), EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))),
				(int)toFahrenheit(tMeat1, EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), "x", 
				(int)toFahrenheit(EEPROMReadInt(PARAMS(ANALOG_V, MEAT2)), EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))),
				(int)toFahrenheit(tMeat2, EEPROMReadInt(PARAMS(BINARY_V, CELCIUS))), "x");
		   break; 
	}
	

	lcd.setCursor(0,0);
	lcd.print(line1);
	lcd.setCursor(0,1);
	lcd.print(line2);
	lcd.setCursor(c % 16, (int)c / 16);


	if (toggle) {
		lcd.noDisplay();
		delay(500);
		lcd.display();
	} else {
		lcd.display();
		delay(500);
	}
}
void receiveEvent(int count)
{

  if (c++>31) c = 0;
  //wdt_reset();
  espReady = true;
  if (count == I2C_MSG_IN_SIZE)
  {
    
    byte cmd = Wire.read();
    byte port = Wire.read();
    int value = Wire.read();
    value += Wire.read()*256;

//#define CMD_DIGITAL_WRITE  1
//#define CMD_DIGITAL_READ   2
//#define CMD_ANALOG_WRITE   3
//#define CMD_ANALOG_READ    4

	/*DEBUGPRINT("cmd: ");
    DEBUGPRINT(cmd);
    DEBUGPRINT(" port: ");
    DEBUGPRINT(port);
    DEBUGPRINT(" value: ");
    DEBUGPRINT_LF(value);*/
    
    switch(cmd)
      {
        case CMD_DIGITAL_WRITE:
          //pinMode(port,OUTPUT);
          //digitalWrite(port,value);
          EEPROMWriteInt(PARAMS(BINARY_V, port), value);
          break;
        case CMD_DIGITAL_READ:
          //pinMode(port,INPUT_PULLUP);
          clearSendBuffer();
          //sendBuffer[0] = digitalRead(port);
          int Status;
          switch(port)
            {
              case SMOKER:
                Status = sSmoker;
              break;
              case MEAT1:
                Status = sMeat1;
              break;
              case MEAT2:
                Status = sMeat2;
              break;
              case SMOKE:
                Status = sSmoke;
              break;
              
            }
          sendBuffer[0] = Status;
          //DEBUGPRINT("Send Status: ");
          //DEBUGPRINT_LF(sendBuffer[0]);
          break;
        case CMD_ANALOG_WRITE:
          //analogWrite(port,value);
          EEPROMWriteInt(PARAMS(ANALOG_V, port), value);
          break;
        case CMD_ANALOG_READ:
          // Only getting delay(1) to set return values
          clearSendBuffer();
          int valueRead; // = analogRead(port);
          switch(port)
            {
              case SMOKER:
                valueRead = tSmoker;
              break;
              case MEAT1:
                valueRead = tMeat1;
              break;
              case MEAT2:
                valueRead = tMeat2;
              break;
              case SMOKE:
                valueRead = dSmoke;
              break;
            }
          sendBuffer[0] = valueRead & 0xff;
          sendBuffer[1] = valueRead >> 8;
          DEBUGPRINT("Send Value: ");
          DEBUGPRINT_LF(valueRead);
          break;
      }
  }
}

void clearSendBuffer()
{
  for(byte x=0; x < sizeof(sendBuffer); x++)
    sendBuffer[x]=0;
}

void requestEvent()
{
  Wire.write((const uint8_t*)sendBuffer,sizeof(sendBuffer));
}

// function to print a device address
void softReset()
{
  asm volatile ("  jmp 0");  
}   


void waitOnESP()
{
  lcd.setCursor(0,0);
  lcd.print("Waiting ESPEasy");
  int x = 10;
  while (x-->1 & !espReady) {
    delay(1000);
  }
  lcd.setCursor(0,1);
  if (espReady) 
    lcd.print("ESPEasy Online");
  else
    lcd.print("ESPEasy OFFLINE");
  delay(1000);
}


