#include <string.h>

#include "phone_forward.h"
#include "trie.h"
#include "lists.h"
#include "memory_management.h"


struct PhoneForward {
  Trie *root;
};

struct PhoneNumbers {
  String *numbers_sequence;
  size_t size;
  size_t allocated_size;
};

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
  size_t num1_len = strlen(num1);
  size_t num2_len = strlen(num2);

  init_string(&num1_string, num1_len);
  init_string(&num2_string, num2_len);
  transfer_chars_to_string(&num1_string, num1, num1_len);
  transfer_chars_to_string(&num2_string, num2, num2_len);

  add_value(pf->root, &num1_string, &num2_string);

  free_string(&num1_string);
  return true;
}

//todo write function that transfers char* contents to my string wrapper
//todo validate possible erros
void phfwdRemove(PhoneForward *pf, char const *num) {
  String num_str;
  size_t num_len = strlen(num);

  init_string(&num_str, strlen(num));

  free_string(&num_str);

}