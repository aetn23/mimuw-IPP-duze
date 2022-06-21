/** @file
 * Interfej modułu pary wskaźnika i liczby.
 * @author Mikołaj Piróg
 * @copyright Mikołaj Piróg
 * @date 2022
 */

#ifndef __PHNUM_PTR_INT_PAIR__
#define __PHNUM_PTR_INT_PAIR__

#include "phone_forward.h"

/**
 * Typ reprezentujący pare wskaźnika i liczby.
 */
typedef struct PhoneNumbersPtrSizeTPair PhoneNumbersPtrSizeTPair;

/**
 * Struktura implementująca parę wskaźnika na @p PhoneNumbers i liczbę typu @p
 * size_t.
 */
struct PhoneNumbersPtrSizeTPair {
  size_t number;     /**< Numer **/
  PhoneNumbers *ptr; /**< Wskaźnik na strukturę @p PhoneNumbers **/
};

/** @brief Tworzy nową strukturę @p PhoneNumbersPtrSizeTPair.
 * Tworzy nową strukturę @p PhoneNumbersPtrSizeTPair. Alokuje pamięć - w razie
 * niepowodzenia zwraca @p NULL.
 * @param[in] number - liczba w parze;
 * @param[in] ptr - wskaźnik w parze;
 * @return Wskaźnik na strukturę @p PhoneNumbersPtrSizeTPair, jeśli alokacja
 * pamięci powiedzie się, @p NULL w przeciwnym wypadku.
 */
PhoneNumbersPtrSizeTPair *init_ptr_size_pair(size_t number, PhoneNumbers *ptr);

/** @brief Zwalnia strukturę @p PhoneNumbersPtrSizeTPair.
 * Zwalnia strukturę @p PhoneNumbersPtrSizeTPair. Nie może się nie udać.
 * @param[in] pair_struct - wskaźnik na strukturę do zwolnienia;
 */
void free_pair_struct(PhoneNumbersPtrSizeTPair *pair_struct);

#endif//__PHNUM_PTR_INT_PAIR__