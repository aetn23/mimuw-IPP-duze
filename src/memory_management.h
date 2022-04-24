#ifndef _MEMORY_MANAGMENT_
#define _MEMORY_MANAGMENT_

#include <stdlib.h>
#include <stdbool.h>


#define START_ARRAY_SIZE 32
#define REALLOC_MULTIPLIER 2

bool check_alloc(void *pointer);

void *malloc_wrapper(size_t size);

void *calloc_wraper(size_t num, size_t size);

void *realloc_wrapper(void *ptr, size_t size);



#endif //_MEMORY_MANAGMENT_
