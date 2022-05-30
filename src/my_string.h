/** @file
 * Interfejs modułu służącego do przechowywania i operowania na napisach.
 *
 * @author Mikołaj Piróg <mp429583@students.mimuw.edu.pl
 * @copyright Mikołaj Piróg
 * @date 2022
 */

#ifndef _MY_STRING_
#define _MY_STRING_

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/**
 * Makro reprezentujące znak zerowy.
 */
#define NULL_CHAR '\0'

/**
 * Marko reprezentujące pusty napis.
 */
#define EMPTY_STRING ""

/**
 * Struktura służąca do przechowywania napisów.
 */
struct String {
  char *content; /**< Tablica znaków. */
  size_t size;   /**< Liczba przechowywanych znaków, nie licząc znaku '\0'. */
  size_t allocated_size; /**< Liczba elementów, na jakie @p content ma miejsc w
                            pamięci. */
};

/**
 * Typ reprezentujący napisy.
 */
typedef struct String String;

/** @brief Inicjuje strukturę.
 * Inicjuje strukturę przechowującą napis. Jeśli @p size jest równe @p 0, nie
 * alokuje pamięci na napis. W przeciwnym razie, alokuje pamięć w rozmiarze
 * pozwalającym przechować @p size znaków.
 * @param[in,out] str - wskaźnik na strukturę przechowującą napis;
 * @param [in] size - liczba mówiącą jak dużo pamięci należy alokować;
 * @return Wartość @p true, jeśli alokacja pamięci powiedzie się, bądź nie
 * będzie jej wcale. Wartość @p false w przeciwnym razie.
 */
bool init_string(String *str, size_t size);

/** @brief Wstawia znak.
 * Wstawia znak @p to_insert w miejscu @p location. Zwiększa zaalokowaną pamięć
 * w razie potrzeby. W wyniku wykonania tej funkcji napis przechowywany w
 * strukturze wskazywaną przez @p str może nie być zakończony @p '\0'.
 * @param[in,out] str - wskaźnik na strukturę przechowującą napis;
 * @param[in] to_insert - znak wstawiany do napisu;
 * @param[in] location - miejsce, gdzie @p to_insert ma być wstawiony;
 * @return Wartość @p false jeśli realokacja pamięci nie powiedzie się.
 * W przeciwnym wypadku @p true.
 */
bool insert_str(String *str, char to_insert, size_t location);

/** @brief Zwalnia strukturę reprezentującą napis.
 * Zwalnia strukturę reprezentującą napis. Ta operacja nie może się nie udać.
 * @param[in,out] str - wskaźnik na strukturę przechowującą napis;
 */
void free_string(String *str);

/** @brief Skleja dwa napisy.
 * Skleja dwa napisy reprezentowane przez wskaźniki @p first, @p second na
 * strukturę reprezentująca napisy. Przy sklejaniu, wynik składa się z całości
 * napisu reprezentowanego przez @p first i od znaków stojących na pozycjach @p
 * from_second @p to_second włącznie z @p second. Wynik wpisywany jest do @p
 * result. Funkcja zakłada poprawność przekazanych parametrów. Jeśli wszystkie
 * operacje powiodą sie, napis reprezentowany przez @p result będzie zakończony
 * @p '\0'. W przeciwnym razie, @p result nie będzie reprezentować poprawnego
 * napisu.
 * @param[in] first - wskaźnik na strukturę reprezentująca pierwszy napis;
 * @param[in] second - wskaźnik na strukturę reprezentująca drugi napis;
 * @param[in] from_second - liczba mówiąca od jakiej pozycji należy uwzględniać
 * znaki z @p second przy sklejaniu;
 * @param[in] to_second - liczba mówiąca do jakiej pozycji należy uwzględniać
 * znaki z @p second przy sklejaniu;
 * @param[in,out] result - wskaźnik na strukturę przechowującą wynik funkcji;
 * @return Wartość @p true, jeśli napisy zostały sklejone. Wartość @p false,
 * jeśli nie udało się zaalokować pamięci.
 */
bool concatenate_from_to(const String *first, const String *second,
                         size_t from_second, size_t to_second, String *result);
/** @brief Sprawdza czy struktura reprezentuje pusty napis.
 * Sprawdza czy struktura reprezentuje pusty napis. Funkcja zakłada poprawność
 * przekazanych argumentów.
 * @param[in] string - wskaźnik na strukturę przechowującą napis;
 * @return Wartość @p true jeśli napis reprezentowany jest pusty. Wartość @p
 * false w przeciwnym wypadku.
 */
bool is_empty_string(const String *string);

/** @brief Zwraca liczbę będącą wartością @p number w tablic ASCII.
 * Zwraca liczbę będącą wartością @p number w tablic ASCII. Funkcja zakłada
 * poprawność przekazanych argumentów, to jest, że @p number jest znakiem z
 * przedziału '0' do '9' włącznie.
 * @param[in] number - znak z przedziału '0' do '9' włącznie;
 * @return Wartość odpowiadająca znaku @p number w tablicy ASCII.
 */
int number_char_to_int(char number);

/** @brief Parsuje znaki reprezentujące numer.
 * Parsuje znaki reprezentujące numer. Kopiuje znaki z @p chars do struktury @p
 * result. W razie potrzeby realokuje pamięć dla napisu w @p result. Ustawia @p
 * memory_failure na wartość @p true, gdy alokacja pamięci nie powiedzie się.
 * Funkcja zakłada poprawność parametru @p result. Wartość @p memory_failure
 * może wynosić @p NULL. Funkcja zakłada, że @p chars jest różne od @p NULL. W
 * razie błędu, @p result nie będzie reprezentować żadnego poprawnego napisu. W
 * przeciwnym razie @p result reprezentuje poprawny napis. Błąd może być nie
 * powiedzeniem się alokacji pamięci albo niereprezentowaniem numeru przez @p
 * chars.
 * @param[in] chars - wskaźnik na ciąg znaków;
 * @param[in,out] result - wskaźnik na strukturę reprezentującą napis;
 * @param[in,out] memory_failure - wskaźnik na wartość logiczną przechowującą
 * informację o wystąpieniu błędu alokacji;
 * @return Wartość @p false jeśli @p chars nie reprezentuje numeru lub gdy
 * alokacja pamięci nie powiedzie się. Wartość @p true w przeciwnym razie.
 */
bool parse_chars_to_string_wrapper(char const *chars, String *result,
                                   bool *memory_failure);

int string_compare(const void *str1, const void *str2);

#endif//_MY_STRING_