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

// This file is to be loaded onto an Arduino Pro Mini so it will act as a simple IO extender to the ESP module.
// Communication between ESP and Arduino is using the I2C bus, so only two wires needed.
// It best to run the Pro Mini on 3V3, although the 16MHz versions do not officially support this voltage level on this frequency.
// That way, you can skip levelconverters on I2C.
// Arduino Mini Pro uses A4 and A5 for I2C bus. ESP I2C can be configured but they are on GPIO-4 and GPIO-5 by default.

//
//  Thermocouples
//
#include "max6675.h"
int thermoCS1 = 4;
int thermoCS2 = 5;
int thermoCS3 = 6;
int thermoDO = 7;
int thermoCLK = 8;
MAX6675 thermocouple1(thermoCLK, thermoCS1, thermoDO);
MAX6675 thermocouple2(thermoCLK, thermoCS2, thermoDO);
MAX6675 thermocouple3(thermoCLK, thermoCS3, thermoDO);

//
//  DHT22
//
#include <DHT.h>
#define DHTPIN            16        // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);


#include <Wire.h>
#define I2C_MSG_IN_SIZE    4
#define I2C_MSG_OUT_SIZE   4

#define CMD_DIGITAL_WRITE  1
#define CMD_DIGITAL_READ   2
#define CMD_ANALOG_WRITE   3
#define CMD_ANALOG_READ    4

#define DEBUG 1

#define DEBUGPRINT_LF(STR) \
  {  \
    if (DEBUG) Serial.println(STR); \
  }
#define DEBUGPRINT(STR) \
  { \
    if (DEBUG) Serial.print(STR); \
  }

volatile uint8_t sendBuffer[I2C_MSG_OUT_SIZE];

void setup()
{
  Wire.begin(0x7f);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  if (DEBUG) Serial.begin(115200);
  delay(10);
  if (DEBUG) Serial.println("Booted...");
  //dht.begin();
}

void loop() {
  
  // basic readout test, just print the current temp
  
   delay(2000);

   Serial.print("1) C = "); 
   Serial.println(thermocouple1.readCelsius());
   Serial.print("2) C = "); 
   Serial.println(thermocouple2.readCelsius());
   Serial.print("3) C = "); 
   Serial.println(thermocouple3.readCelsius());

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//    byte chk;
    //status = STATUS_ERROR;
//    chk = dht.read(DHTPIN);
//    if (chk == DHTLIB_OK) {
//      int temp1;
//      int temp2;
//      temp1 = abs((dht.temperature - (int)dht.temperature) * 100);
//      temp2 = (dht.humidity - (int)dht.humidity) * 100;
//    }

  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  //if (isnan(h) || isnan(t)) {
    //Serial.println("Failed to read from DHT sensor!");
    //return;
  //}

  Serial.print("Hum:   ");
  Serial.println(h);
  Serial.print("4) C = "); 
  Serial.println(t);
  Serial.println();
     
}

void receiveEvent(int count)
{
  if (count == I2C_MSG_IN_SIZE)
  {
    byte cmd = Wire.read();
    byte port = Wire.read();
    int value = Wire.read();
    if (DEBUG) DEBUGPRINT("cmd: ");
    if (DEBUG) DEBUGPRINT(cmd);
    if (DEBUG) DEBUGPRINT(" port: ");
    if (DEBUG) DEBUGPRINT(port);
    if (DEBUG) DEBUGPRINT(" value: ");
    if (DEBUG) DEBUGPRINT_LF(value);
    
    value += Wire.read()*256;
    switch(cmd)
      {
        case CMD_DIGITAL_WRITE:
          //pinMode(port,OUTPUT);
          //digitalWrite(port,value);
          break;
        case CMD_DIGITAL_READ:
          //pinMode(port,INPUT_PULLUP);
          //clearSendBuffer();
          //sendBuffer[0] = digitalRead(port);
          if (DEBUG) DEBUGPRINT("Send: ");
          if (DEBUG) DEBUGPRINT_LF(sendBuffer[0]);
          break;
        case CMD_ANALOG_WRITE:
          //analogWrite(port,value);
          break;
        case CMD_ANALOG_READ:
          //clearSendBuffer();
          // int valueRead = analogRead(port);
          // sendBuffer[0] = valueRead & 0xff;
          // sendBuffer[1] = valueRead >> 8;
		  
		    // request to all devices on the bus
		  //Serial.print("Requesting temperatures...");
		  //sensors.requestTemperatures(); // Send the command to get temperatures
		  //Serial.println("DONE");
  
		  // It responds almost immediately. Let's print out the data
		//float tempC = sensors.getTempC(insideThermometer);
		//DEBUGPRINT_LF(tempC);
		//int temp1 = abs((tempC - (int)tempC) * 100);
		//DEBUGPRINT_LF(temp1);
      //  sendBuffer[0] = (int)tempC & 0xff;
      //  sendBuffer[1] = (int)tempC >> 8;
      //  sendBuffer[2] = (int)temp1 & 0xff;
      //  sendBuffer[3] = (int)temp1 >> 8;
		//sprintf(sendBuffer, "Temp: %0d.%d", (int)tempC, temp1);
		//DEBUGPRINT_LF(sendBuffer);
        if (DEBUG) DEBUGPRINT("Send: ");
        if (DEBUG) DEBUGPRINT(sendBuffer[0]);
        if (DEBUG) DEBUGPRINT(sendBuffer[1]);
        if (DEBUG) DEBUGPRINT(".");
        if (DEBUG) DEBUGPRINT(sendBuffer[2]);
        if (DEBUG) DEBUGPRINT_LF(sendBuffer[3]);
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

