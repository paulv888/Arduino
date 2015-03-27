#include "Arduino.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.println("AT");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Open Serial Bridge");
}

void loop() // run over and over
{

	if (mySerial.available()) {
		char data = mySerial.read();
		Serial.write(data);
	}
  
	if (Serial.available()) {
		char data = Serial.read();
		mySerial.write(data);
	}
}

