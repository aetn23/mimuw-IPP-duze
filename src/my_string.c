/** @file
 * Implementacja modułu do przechowywania i operowania na napisach.
 *
 * @author Mikołaj Piróg <mp429583@students.mimuw.edu.pl
 * @copyright Mikołaj Piróg
 * @date 2022
 */

#include <ctype.h>

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

bool insert_str(String *str, const char to_insert, const size_t location) {
  while (location >= str->allocated_size) {
    char *tmp = realloc(str->content, sizeof(char) * str->allocated_size *
                                              REALLOC_MULTIPLIER);

    if (!check_alloc(tmp))
      return false;

    str->allocated_size *= REALLOC_MULTIPLIER;
    str->content = tmp;
  }

  str->content[location] = to_insert;
  str->size++;

  return true;
}

/** @brief Zakańcza napis przechowywany przez strukturę @p String znakiem @p
 * '\0'.
 *  Zakańcza napis przechowywany przez strukturę @p String znakiem @p
 * '\0'. Długość napisu nie zmienia się, zachowując konwencję obecną w C. W
 * razie potrzeby alokuje pamięć. Zakłada poprawność argumentów.
 * @param[in,out] str - wskaźnik na strukturę przechowującą napis;
 * @return Wartość @p true, jeśli operacja powiedzie się. Wartość @p false,
 * jeśli nie uda się zaalokować pamięci.
 */
static bool null_terminate(String *str) {
  if (!insert_str(str, NULL_CHAR, str->size))
    return false;

  str->size--;

  return true;
}

void free_string(String *str) {
  if (str->content != NULL)
    free(str->content);
}

bool concatenate_from_to(const String *first, const String *second,
                         const size_t from_second, const size_t to_second,
                         String *result) {
  for (size_t i = 0; i < first->size; i++) {
    if (!insert_str(result, first->content[i], result->size))
      return false;
  }

  for (size_t i = from_second; i <= to_second; i++) {
    if (!insert_str(result, second->content[i], result->size))
      return false;
  }

  return null_terminate(result);
}

bool is_empty_string(const String *string) {
  return !(bool) strcmp(string->content, EMPTY_STRING);
}

int number_char_to_int(const char number) { 
  if (number == '*')
    return 10;
  if (number == '#')
    return 11;
  else
    return (int) (number - '0');
}

static bool is_proper_digit(char digit) {
  if (digit == '*' || digit == '#' || (digit >= '0' && digit <= '9'))
    return true;
  return false;
}

bool parse_chars_to_string_wrapper(char const *chars, String *result,
                                   bool *memory_failure) {
  for (size_t i = 0; chars[i] != NULL_CHAR; i++) {
    if (!is_proper_digit(chars[i]))
      return false;

    if (!insert_str(result, chars[i], result->size)) {
      if (memory_failure != NULL)
        *memory_failure = true;

      return false;
    }
  }

  return null_terminate(result);
}

int string_compare(const void *str1, const void *str2) {
  if(((String*)str1)->content == NULL && ((String*)str2)->content == NULL)
    return 0;
  else if (((String*)str1)->content)
   -1;
  return strcmp(((String*)str1)->content, ((String*)str2)->content);
}