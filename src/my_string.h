#ifndef _MY_STRING_
#define _MY_STRING_

#include <stdbool.h>
#include <stddef.h>

#define NULL_CHAR '\0'

typedef struct String {
  char *content;
  size_t size;
  size_t allocated_size;
} String;

void init_string(String *str, size_t size);

void insert_str(String *str, char to_insert, size_t location);

void clear_str(String *str);

#endif //_MY_STRING_