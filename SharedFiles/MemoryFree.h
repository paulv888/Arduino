#ifndef	MEMORY_FREE_H
#define MEMORY_FREE_H
#include <Arduino.h>


#ifdef __cplusplus
extern "C" {
#endif

long check_mem();
void printMem(char *name);

#ifdef  __cplusplus
}
#endif

#endif
