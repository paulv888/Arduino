/*
 * ArrayHandler.h
 *
 *  Created on: Oct 13, 2012
 *      Author: pvloon
 */

#ifndef ARRAYHANDLER_H_
#define ARRAYHANDLER_H_

#include <stdlib.h>

void * operator new[](size_t size);
void operator delete[](void * ptr);

#endif /* ARRAYHANDLER_H_ */
