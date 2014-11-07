// On Arduino Duemilanove with ATmega328:
//
// Reported free memory with str commented out:
// 1824 bytes
//
// Reported free memory with str and Serial.println(str) uncommented:
// 1810
//
// Difference: 14 bytes (13 ascii chars + null terminator)

#include "MemoryFree.h"


byte * heapptr, * stackptr;
long check_mem() {
  stackptr = (byte *)malloc(4);          // use stackptr temporarily
  heapptr = stackptr;                     	// save value of heap pointer
  free(stackptr);      						// free up the memory again (sets stackptr to 0)
  stackptr =  (byte *)(SP);           	// save value of stack pointer
  return long(stackptr) - long(heapptr);
}

void printMem(char *name) {
	Serial.print(name);
	Serial.print(" free ");
	Serial.print(check_mem());
	Serial.print(" heapptr ");
	Serial.print((long)heapptr);
	Serial.print(" stackptr ");
	Serial.println((long)stackptr);
}
