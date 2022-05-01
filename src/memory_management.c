#include "memory_management.h"

bool check_alloc(void *pointer) {
  if (pointer == NULL)
    return false;
  return true;
}