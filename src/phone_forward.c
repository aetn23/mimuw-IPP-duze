#include <string.h>

#include "phone_forward.h"
#include "trie.h"
#include "memory_management.h"


struct PhoneForward {
  Trie *root;
};

struct PhoneNumbers {
  String *numbers_sequence;
  size_t size;
  size_t allocated_size;
};

PhoneNumbers *init_phone_numbers(size_t size) {
    PhoneNumbers *result = malloc(sizeof(PhoneNumbers));
    if (!check_alloc(result))
      return NULL;

    if (size != 0) {
    result->numbers_sequence = malloc(sizeof(String) * size);

    if (!check_alloc(result->numbers_sequence))
      return NULL;

    result->size = 0;
    result->allocated_size = size;
  } else {
    result->numbers_sequence = NULL;
    result->size = 0;
    result->allocated_size = 0;
  }
  return result;
}

bool push_back_numbers(PhoneNumbers *numbers, String *number) {
  if (numbers->size == numbers->allocated_size) {
    String *new_array = realloc(numbers->numbers_sequence, numbers->allocated_size*REALLOC_MULTIPLIER*sizeof(String));

    if (!check_alloc(new_array)) {
      return false;
    } 

    numbers->numbers_sequence = new_array;
    numbers->allocated_size *= REALLOC_MULTIPLIER;
  }

  numbers->numbers_sequence[numbers->size] = *number;
  numbers->size++;

  return true;
}

void phnumDelete(PhoneNumbers *numbers) {
  if (numbers == NULL)
    return;

  for (size_t i = 0; i < numbers->size; i++)
    free_string(&numbers->numbers_sequence[i]);

  free(numbers->numbers_sequence);

  free(numbers);
}

PhoneForward *phfwdNew() {
  PhoneForward *result = malloc(sizeof(PhoneForward));
  if (!check_alloc(result))
    return NULL;

  Trie *trie;
  if (!init_trie(&trie, NULL_CHAR, EMPTY_STRING, 0)) {
    return NULL;
  }

  result->root = trie;
  return result;
}

void phfwdDelete(PhoneForward *pf) {
  if (pf == NULL)
    return;

  free_trie(pf->root);
  free(pf);
}
//todo take into account errors
bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
  String num1_string;
  String num2_string;
  init_string(&num1_string, START_ARRAY_SIZE);
  init_string(&num2_string, START_ARRAY_SIZE);

  if (!parse_chars_to_string_wrapper(num1, &num1_string))
    return false;

  if (!parse_chars_to_string_wrapper(num2, &num2_string))
    return false;

  add_value(pf->root, &num1_string, &num2_string);

  free_string(&num1_string);
  return true;
}

//todo write function that transfers char* contents to my string wrapper
//todo validate possible erros
void phfwdRemove(PhoneForward *pf, char const *num) {
  remove_subtree(&pf->root, num);
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
  if (pnum == NULL || idx >= pnum->size)
    return NULL;
  return pnum->numbers_sequence[idx].content;
}

//todo validate inout
PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
  PhoneNumbers *result = init_phone_numbers(1);

  String num_str;
  init_string(&num_str, START_ARRAY_SIZE);
  parse_chars_to_string_wrapper(num, &num_str);

  String forwarded_number;
  init_string(&forwarded_number, START_ARRAY_SIZE);

  get_deepest_nonempty_value(pf->root, &num_str, &forwarded_number);

  push_back_numbers(result, &forwarded_number);

  free_string(&num_str);

  return result;
}