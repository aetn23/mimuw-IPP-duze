#include "memory_management.h"
#include "my_string.h"



void init_string(String *str, const size_t size) {
  if (size != 0) {
    str->content = malloc_wrapper(sizeof(char) * START_ARRAY_SIZE);
    str->size = 0;
    str->allocated_size = START_ARRAY_SIZE;
  } else {
    str->content = NULL;
    str->size = 0;
    str->allocated_size = START_ARRAY_SIZE;
  }
}

// There is no guarantee that string wil be null terminated after this.
void insert_str(String *str, const char to_insert, const size_t location) {
  while (location >= str->allocated_size) {
    str->allocated_size *= REALLOC_MULTIPLIER;
    str->content =
        realloc_wrapper(str->content, sizeof(char) * str->allocated_size);
  }

  str->content[location] = to_insert;
  str->size++;
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

void transfer_chars_to_string (String *string, char *chars, size_t chars_len) {
  for (size_t i = 0; i < chars_len; i++) {
    insert_str(string, chars[i], string->size);
  }
}