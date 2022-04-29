#include "memory_management.h"
#include "my_string.h"



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
    char *tmp = realloc_wrapper(str->content, sizeof(char) * str->allocated_size);

    if (!check_alloc(tmp))
      return false;

    str->allocated_size *= REALLOC_MULTIPLIER;
    str->content = tmp;
  }

  str->content[location] = to_insert;
  str->size++;
  return true;
}

void clear_str(String *str) {
  for (size_t i = 0; i < str->size; i++)
    str->content[i] = NULL_CHAR;
  str->size = 0;
}

void free_string(String *str) {
  if (str->content != NULL)
    free(str->content);
}

bool transfer_chars_to_string (String *string, char const *chars, size_t chars_len) {
  for (size_t i = 0; i < chars_len; i++) {
    if (!insert_str(string, chars[i], string->size))
      return false;
  }
  return true;
}