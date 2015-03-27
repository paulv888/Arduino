// On Arduino Duemilanove with ATmega328:
//
// Reported free memory with str commented out:
// 1824 bytes
//
// Reported free memory with str and dbg.println(str) uncommented:
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
	dbg.print(name);
	dbg.print("Free ");
	dbg.print(check_mem());
	dbg.print(" Heap ");
	dbg.print((long)heapptr);
	dbg.print(" Stack ");
	dbg.println((long)stackptr);
}

