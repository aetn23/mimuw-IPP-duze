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

bool init_string(String *str, size_t size);

bool insert_str(String *str, char to_insert, size_t location);

void clear_str(String *str);

void free_string(String *str);

bool transfer_chars_to_string(String *string, char *chars, size_t chars_len);

#endif //_MY_STRING_