#ifndef _MEMORY_MANAGMENT_
#define _MEMORY_MANAGMENT_

#include <stdlib.h>
#include <stdbool.h>

//todo change this constants
#define START_ARRAY_SIZE_SMALL 1
#define START_ARRAY_SIZE_BIG 1
#define REALLOC_MULTIPLIER 2

bool check_alloc(void *pointer);

#endif //_MEMORY_MANAGMENT_
