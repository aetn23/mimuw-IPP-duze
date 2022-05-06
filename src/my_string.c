#include <ctype.h>

#include "my_string.h"
#include "memory_management.h"


bool init_string(String *str, const size_t size) {
  if (size != 0) {
    str->content = malloc(sizeof(char) * size);

    if (!check_alloc(str->content))
      return false;

    str->size = 0;
    str->allocated_size = size;
  } else {
    str->content = NULL;
    str->size = 0;
    str->allocated_size = 0;
  }
  return true;
}

// There is no guarantee that string wil be null terminated after this.
bool insert_str(String *str, const char to_insert, const size_t location) {
  while (location >= str->allocated_size) {
    char *tmp = realloc(str->content, sizeof(char) * str->allocated_size * REALLOC_MULTIPLIER);

    if (!check_alloc(tmp))
      return false;

    str->allocated_size *= REALLOC_MULTIPLIER;
    str->content = tmp;
  }

  str->content[location] = to_insert;
  str->size++;
  return true;
}

bool null_terminate(String *str) {
  if (!insert_str(str, NULL_CHAR, str->size))
    return false;

  str->size--;
  return true;
}

void free_string(String *str) {
  if (str->content != NULL)
    free(str->content);
}

bool transfer_chars_to_string(String *string, char const *chars, const size_t chars_len) {
  for (size_t i = 0; i < chars_len; i++) {
    if (!insert_str(string, chars[i], i))
      return false;
  }
  if (!insert_str(string, NULL_CHAR, string->size))
    return false;
  string->size--;
  return true;
}

bool concatate_from_to(const String *first, const String *second, const size_t from, const size_t to, String *result) {
  for (size_t i = 0; i < first->size; i++) {
    if (!insert_str(result, first->content[i], result->size))
      return false;
  }

  for (size_t i = from; i <= to; i++) {
    if (!insert_str(result, second->content[i], result->size))
      return false;
  }

  return null_terminate(result);
}

bool is_empty_string (const String *string) {
  return !(bool)strcmp(string->content, EMPTY_STRING);
}

int number_char_to_int (const char number) {
  return (int)(number - '0');
}

bool parse_chars_to_string_wrapper(char const *chars, String *result, bool *memory_failure) {
  for (size_t i = 0; chars[i] != NULL_CHAR; i++) {
    if (!isdigit(chars[i]))
      return false;

    if(!insert_str(result, chars[i], result->size)) {
      if (memory_failure != NULL)
        *memory_failure = true;
      return false;
    }
  }

  return null_terminate(result);
}