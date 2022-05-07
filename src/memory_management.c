/** @file
 * Implementacja funkcji do zarządzania pamięcią.
 *
 * @author Mikołaj Piróg <mp429583@students.mimuw.edu.pl
 * @copyright Mikołaj Piróg
 * @date 2022
 */

#include "memory_management.h"

bool check_alloc(void *pointer) {
  if (pointer == NULL)
    return false;

  return true;
}