/** @file
 * Interfejs struktury służącej do przechowywania i operowania na napisach.
 *
 * @author Mikołaj Piróg <mp429583@students.mimuw.edu.pl
 * @copyright Uniwerstyet Warszawski
 * @date 2022
 */

#ifndef _MY_STRING_
#define _MY_STRING_

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/**
 * Makra reprezentujące szczególne znaki bądź napisy.
 */
#define NULL_CHAR '\0'
#define EMPTY_STRING ""

/**
 * Struktura służąca do przechowywania napisów.
 */
typedef struct String {
  char *content;
  size_t size;
  size_t allocated_size;
} String;

/** @brief Inicjuje strukturę.
 * Inicjuje strukturę przechowującą napis. Jeśli @p size jest równe @p 0, nie
 * alokuje pamięci na napis. W przeciwnym razie, alokuje pamięć w rozmiarze
 * pozwalającym przechować @p size znaków.
 * @param[in,out] str - wskaźnik na strukturę przechowującą napis;
 * @param [in] size - liczba mówiącą jak dużo pamięci należy alokować.
 * @return Wartość @p true, jeśli alokacja pamięci powiedzie się.
 * Wartość @p false w przeciwnym razie.
 */
bool init_string(String *str, size_t size);

/** @brief Wstawia znak.
 * Wstawia znak @p to_insert w miejscu @p location. Zwiększone
 * @param str
 * @param to_insert
 * @param location
 * @return
 */
bool insert_str(String *str, char to_insert, size_t location);

void clear_str(String *str);

void free_string(String *str);

bool transfer_chars_to_string(String *string, char const *chars, size_t chars_len);

bool concatate_from_to(const String *first, const String *second, size_t from, size_t to, String *result);

bool is_empty_string(const String *string);

int number_char_to_int(char number);

bool parse_chars_to_string_wrapper(char const *chars, String *result, bool *memory_failure);

#endif//_MY_STRING_