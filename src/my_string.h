#ifndef _MY_STRING_
#define _MY_STRING_

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define NULL_CHAR '\0'
#define EMPTY_STRING ""

typedef struct String {
  char *content;
  size_t size;
  size_t allocated_size;
} String;

bool init_string(String *str, size_t size);

bool insert_str(String *str, char to_insert, size_t location);

void clear_str(String *str);

void free_string(String *str);

bool transfer_chars_to_string(String *string, char const *chars, size_t chars_len);

bool concatate_from_to(const String *first, const String *second, const size_t from, const size_t to, String *result);

bool is_empty_string (const String *string);

int number_char_to_int (char number);

bool parse_chars_to_string_wrapper(char const *chars, String *result);

#endif //_MY_STRING_