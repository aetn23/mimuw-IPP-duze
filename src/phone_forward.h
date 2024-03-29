/** @file
 * Interfejs modułu obsługującego przekierowania numerów telefonicznych
 *
 * @author Marcin Peczarski <marpe@mimuw.edu.pl>
 * @author Mikołaj Piróg <mp429583@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __PHONE_FORWARD_H__
#define __PHONE_FORWARD_H__

#include <stdbool.h>
#include <stddef.h>

#include "my_string.h"


/**
 * To jest deklaracja struktury przechowującej przekierowania numerów telefonów.
 */
struct PhoneForward;

/**
 * Typ reprezentujący przekierowania numerów telefonu.
 */
typedef struct PhoneForward PhoneForward;

/**
 * To jest deklaracja struktury przechowującej ciąg numerów telefonów.
 */
struct PhoneNumbers;

/**
 * Typ reprezentujący ciąg numerów telefonów.
 */
typedef struct PhoneNumbers PhoneNumbers;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
PhoneForward *phfwdNew(void);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf – wskaźnik na usuwaną strukturę.
 */
void phfwdDelete(PhoneForward *pf);

/** @brief Dodaje przekierowanie.
 * Dodaje przekierowanie wszystkich numerów mających prefiks @p num1, na numery,
 * w których ten prefiks zamieniono odpowiednio na prefiks @p num2. Każdy numer
 * jest swoim własnym prefiksem. Jeśli wcześniej zostało dodane przekierowanie
 * z takim samym parametrem @p num1, to jest ono zastępowane.
 * Relacja przekierowania numerów nie jest przechodnia.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num1   – wskaźnik na napis reprezentujący prefiks numerów
 *                     przekierowywanych;
 * @param[in] num2   – wskaźnik na napis reprezentujący prefiks numerów,
 *                     na które jest wykonywane przekierowanie.
 * @return Wartość @p true, jeśli przekierowanie zostało dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne lub nie udało
 *         się alokować pamięci.
 */
bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2);

/** @brief Usuwa przekierowania.
 * Usuwa wszystkie przekierowania, w których parametr @p num jest prefiksem
 * parametru @p num1 użytego przy dodawaniu. Jeśli nie ma takich przekierowań
 * lub napis nie reprezentuje numeru, nic nie robi.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num    – wskaźnik na napis reprezentujący prefiks numerów.
 */
void phfwdRemove(PhoneForward *pf, char const *num);

/** @brief Wyznacza przekierowanie numeru.
 * Wyznacza przekierowanie podanego numeru. Szuka najdłuższego pasującego
 * prefiksu. Wynikiem jest ciąg zawierający co najwyżej jeden numer. Jeśli dany
 * numer nie został przekierowany, to wynikiem jest ciąg zawierający ten numer.
 * Jeśli podany napis nie reprezentuje numeru, wynikiem jest pusty ciąg.
 * Alokuje strukturę @p PhoneNumbers, która musi być zwolniona za pomocą
 * funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci.
 */
PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num);

/** @brief Wyznacza przekierowania na dany numer.
 * Wyznacza następujący ciag odwrotności @p num. Odwrotność jest rozumiana jako
 * takie num, że phfwdGet(pf, num) daje strukturę @p PhoneNumbers
 * przechowująca numer równy @p num. Dodatkowo ciąg
 * wynikowy zawsze zawiera też numer @p num. Wynikowe numery są posortowane
 * leksykograficznie i nie mogą się powtarzać. Jeśli podany napis nie
 * reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers, która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci.
 */
PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum – wskaźnik na usuwaną strukturę.
 */
void phnumDelete(PhoneNumbers *pnum);

/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera.
 * @param[in] pnum – wskaźnik na strukturę przechowującą ciąg numerów telefonów;
 * @param[in] idx  – indeks numeru telefonu.
 * @return Wskaźnik na napis reprezentujący numer telefonu. Wartość NULL, jeśli
 *         wskaźnik @p pnum ma wartość NULL lub indeks ma za dużą wartość.
 */
char const *phnumGet(PhoneNumbers const *pnum, size_t idx);


/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę przechowującą numery telefonu. Alokuje pamięć na @p
 * size numerów telefonu. Jeśli @p size ma wartość zero, nie alokuje pamięci.
 * @param[in] size - liczba numerów telefonów, na jaką struktura ma mieć pamięć;
 * @return Wskaźnik na utworzoną strukturę, lub @p NULL gdy nie udało się
 * zaalokować pamięci.
 */
PhoneNumbers *phnumNew(size_t size);


/** @brief Wstawia numer telefonu do struktury.
 * Wstawia numer telefonu do struktury. Alokuje pamięć, jeśli zachodzi taka
 * potrzeba. Funkcja zakłada poprawność parametrów. Po nieudanej alokacji
 * struktura pod wskaźnikiem @p numbers nadal jest poprawna.
 * @param[in,out] numbers - wskaźnik na strukturę przechowującą numery telefonu;
 * @param[in] number - numer telefonu;
 * @return Wartość @p true, jeśli operacje powiodą się. Wartość @p false, jeśli
 * ewentualna alokacja pamięci nie powiedzie się.
 */
bool push_back_numbers(PhoneNumbers *numbers, const String *number);

/** @brief Znajduje pierwszy napis ze struktury @p pnum, różny od @p NULL i o
 * indeksie większym od @p idx.
 * Znajduje pierwszy napis ze struktury @p pnum, różny od @p NULL i o
 * indeksie większym od @p idx. Jeśli taki nie istnieje, lub argumenty nie są
 * prawidłowe, zwraca @p NULL.
 * @param[in] pnum - wskaźnik na strukturę PhoneNumbers;
 * @param[in] idx - indeks szukanego napisu;
 * @return Wskaźnik na strukturę reprezentującą napis, który spełnia warunki
 * opisane wyżej. Jeśli taki nie istnieje, wynik to @p NULL.
 */
String *phnum_get_string(const PhoneNumbers *pnum, size_t idx);

/** @brief Zwraca rozmiar struktury @p pnum.
 * Zwraca rozmiar struktury @p pnum, czyli liczbę elementów jakie przechowuje.
 * Zakłada poprawność argumentów.
 * @param[in] pnum - Wskaźnik na strukturę PhoneNumbers;
 * @return Rozmiar wskazywanej przez @p pnum struktury.
 */
size_t pnum_size(const PhoneNumbers *pnum);

/** @brief Wyznacza przeciwobraz funkcji @p phfwdGet.
 * Wyznacza przeciwobraz funkcji @p phfwdGet. Zwraca wskaźnik na strukturę @p
 * PhoneNumbers, zawierającą takie numery x, że @p phfwdGet(x) = @p num.
 * @param[in] pf - Wskaźnik na strukturę PhoneForward;
 * @param[in] num - wskaźnik na ciąg znaków reprezentujący numer;
 * @return Wskaźnik na strukturę reprezentującą napis, który spełnia warunki
 * opisane wyżej. @p NULL, gdy alokacja pamięci nie powietrze się. Gdy @p num
 * nie reprezentuje numeru, wynik to pusta struktura @p PhoneNumbers.
 */
PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num);

#endif /* __PHONE_FORWARD_H__ */
