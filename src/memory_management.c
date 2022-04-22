#include "memory_management.h"

void check_alloc(void *pointer) {
  if (pointer == NULL)
    //todo handle_alloc_failure();
}

void *malloc_wrapper(const size_t size) {
  void *result = malloc(size);
  check_alloc(result);

  return result;
}

void *calloc_wraper(const size_t num, const size_t size) {
  void *result = calloc(num, size);
  check_alloc(result);

  return result;
}

void *realloc_wrapper(void *ptr, const size_t size) {
  ptr = realloc(ptr, size);
  check_alloc(ptr);

  return ptr;
}

