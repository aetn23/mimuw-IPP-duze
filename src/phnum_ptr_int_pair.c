/** @file
* Implementacja modułu pary wskaźnika i liczby.
* @author Mikołaj Piróg
* @copyright Mikołaj Piróg
* @date 2022
*/

#include "phnum_ptr_int_pair.h"
#include "memory_management.h"

PhoneNumbersPtrSizeTPair *init_ptr_size_pair(size_t number, PhoneNumbers *ptr) {
  PhoneNumbersPtrSizeTPair *result = malloc(sizeof(PhoneNumbersPtrSizeTPair));
  if (!check_alloc(result))
    return NULL;

  result->number = number;
  result->ptr = ptr;

  return result;
}

void free_pair_struct(PhoneNumbersPtrSizeTPair *pair_struct) {
  free(pair_struct);
}
