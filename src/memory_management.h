/** @file
 * Interfejs makr i funkcji związanych z zarządzaniem pamięcią.
 *
 * @author Mikołaj Piróg <mp429583@students.mimuw.edu.pl
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef _MEMORY_MANAGMENT_
#define _MEMORY_MANAGMENT_

#include <stdbool.h>
#include <stdlib.h>

/**
 * Makro oznaczające początkową wielkość tablic.
 */
#define START_ARRAY_SIZE_SMALL 1
/**
 * Marko oznaczające stałą powiększania tablicy.
 */
#define REALLOC_MULTIPLIER 2

/** @brief Sprawdza, czy alokacja pamięci się powiodła.
 * Sprawdza, czy @p pointer jest poprawnym wskaźnikiem do pewnego miejsca w
 * pamięci.
 * @param[in] pointer - wskaźnik dowolnego typu;
 * @return Wartość @p true, jeśli wskaźnik jest różny od NULL. Wartość @p false
 * w przeciwnym wypadku.
 */
bool check_alloc(void *pointer);

#endif//_MEMORY_MANAGMENT_
