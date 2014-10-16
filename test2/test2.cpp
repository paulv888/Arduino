// Do not remove the include below
#include "test2.h"




//#define P(name)   static const prog_uchar name[] PROGMEM


P(DeviceID) = "DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>DID>";
P(CommandID) = "CID>";
P(Heat) = "SNS>";

static const prog_char test[] PROGMEM = "123";

int printP(const prog_char *str, bool getLen = false) {
	// copy data out of program memory into local storage, write out in
	// chunks of 32 bytes to avoid extra short TCP/IP packets
	if (getLen) {
		return strlen_P(str);
	}

	uint8_t buffer[32];
	size_t bufferEnd = 0;
	while (buffer[bufferEnd++] = pgm_read_byte(str++)) {
		if (bufferEnd == 32) {
			Serial.write(buffer, 32);
			bufferEnd = 0;
		}
	}

	// write out everything left but trailing NUL
	if (bufferEnd > 1) Serial.write(buffer, bufferEnd - 1);
}
void setup() {
	Serial.begin(57600);


}



void loop() {

printP(DeviceID);
printP(CommandID);
printP(Heat);


Serial.println (printP(DeviceID,true));
Serial.println (printP(CommandID,true));
Serial.println (printP(Heat,true));


delay (1000);

}
