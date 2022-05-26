/** @file
 * Implementacja modułu obsługującego przekierowania numerów telefonicznych
 *
 * @author Mikołaj Piróg <mpr429583@students.mimuw.edu.pl>
 * @copyright Mikołaj Piróg
 * @date 2022
 */
#include <string.h>

#include "memory_management.h"
#include "phone_forward.h"
#include "trie.h"

/**
 * Implementacja Struktury służącej do przechowywania przekierowań.
 * numerów telefonów.
 */
struct PhoneForward {
  Trie *root; /**< Wskaźnik na korzeń drzewa Trie. */
  Trie *reverse_trie_root;
};

/**
 * Implementacja struktury przechowującej numery telefonów.
 */
struct PhoneNumbers {
  String *numbers_sequence; /**< Tablica struktur reprezentujących napisy. */
  size_t size; /**< Liczba elementów w tablicy @p numbers_sequence. */
  size_t allocated_size; /**< Liczba elementów, na jakie @p numbers_sequence ma
                            miejsce w pamięci. */
};

PhoneNumbers *phnumNew(size_t size) {
  PhoneNumbers *result = malloc(sizeof(PhoneNumbers));
  if (!check_alloc(result))
    return NULL;

  if (size != 0) {
    result->numbers_sequence = malloc(sizeof(String) * size);

    if (!check_alloc(result->numbers_sequence)) {
      free(result);

      return NULL;
    }

    result->size = 0;
    result->allocated_size = size;
  } else {
    result->numbers_sequence = NULL;
    result->size = 0;
    result->allocated_size = 0;
  }

  return result;
}

/** @brief Wstawia numer telefonu do struktury.
 * Wstawia numer telefonu do struktury. Alokuje pamięć, jeśli zachodzi taka
 * potrzeba. Funkcja zakłada poprawność parametrów. Po nieudanej alokacji
 * struktura pod wskaźnikiem @p numbers nadal jest poprawna.
 * @param[in,out] numbers - wskaźnik na strukturę przechowującą numery telefonu;
 * @param[in] number - numer telefonu;
 * @return Wartość @p true, jeśli operacje powiodą się. Wartość @p false, jeśli
 * ewentualna alokacja pamięci nie powiedzie się.
 */
bool push_back_numbers(PhoneNumbers *numbers, const String *number) {
  if (numbers->size == numbers->allocated_size) {
    String *new_array = realloc(numbers->numbers_sequence,
                                numbers->allocated_size * REALLOC_MULTIPLIER *
                                        sizeof(String));

    if (!check_alloc(new_array))
      return false;

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
  if (!init_trie(&trie, NULL_CHAR, NULL, false)) {
    free(result);

    return NULL;
  }

  Trie *reverse_trie;
  if (!init_trie(&reverse_trie, NULL_CHAR, NULL, true)) {
    free(result);
    free(trie);

    return NULL;
  }

  result->root = trie;
  result->reverse_trie_root = reverse_trie;

  return result;
}

void phfwdDelete(PhoneForward *pf) {
  if (pf == NULL)
    return;

  free_trie(pf->reverse_trie_root);
  free_trie(pf->root);
  free(pf);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
  if (pf == NULL || num1 == NULL || num2 == NULL)
    return false;

  String num1_string;
  String num2_string;

  if (!init_string(&num1_string, START_ARRAY_SIZE_SMALL))
    return false;

  if (!init_string(&num2_string, START_ARRAY_SIZE_SMALL)) {
    free_string(&num1_string);

    return false;
  }

  if (!parse_chars_to_string_wrapper(num1, &num1_string, NULL) ||
      !parse_chars_to_string_wrapper(num2, &num2_string, NULL) ||
      !strcmp(num1, num2) || is_empty_string(&num1_string) ||
      is_empty_string(&num2_string)) {
    free_string(&num1_string);
    free_string(&num2_string);

    return false;
  }

  Trie *trie_result = add_value(pf->root, &num1_string, &num2_string);
  if(!check_alloc(trie_result)) {
    free_string(&num1_string);
    free_string(&num2_string);

    return false;
  }

  Trie *reverse_trie_result = add_value(pf->reverse_trie_root, &num2_string, &num1_string);
  if(!check_alloc(reverse_trie_result)) {
    free_string(&num1_string);
    free_string(&num2_string);
    free_trie(trie_result);

    return false;
  }

  trie_result->ptr_to_node_in_reverse_trie = reverse_trie_result;
  //set trie


  //free_string(&num1_string);

  return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
  if (num != NULL && pf != NULL)
    remove_subtree(&pf->root, num);
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
  if (pnum == NULL || idx >= pnum->size)
    return NULL;

  return pnum->numbers_sequence[idx].content;
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
  if (pf == NULL)
    return NULL;

  String num_str;
  String forwarded_number;
  PhoneNumbers *result = phnumNew(1);

  if (!check_alloc(result))
    return NULL;

  if (num == NULL)
    return result;

  if (!init_string(&num_str, START_ARRAY_SIZE_SMALL)) {
    phnumDelete(result);

    return NULL;
  }

  bool memory_failure = false;
  if (!parse_chars_to_string_wrapper(num, &num_str, &memory_failure)) {
    if (!memory_failure) {
      free_string(&num_str);

      return result;
    }

    phnumDelete(result);
    free_string(&num_str);

    return NULL;
  }

  if (is_empty_string(&num_str)) {
    free_string(&num_str);

    return result;
  }

  if (!init_string(&forwarded_number, START_ARRAY_SIZE_SMALL)) {
    free_string(&num_str);
    phnumDelete(result);

    return NULL;
  }

  if (!get_num_forward_from_trie(pf->root, &num_str, &forwarded_number) ||
      !push_back_numbers(result, &forwarded_number)) {
    free_string(&num_str);
    free_string(&forwarded_number);
    phnumDelete(result);

    return NULL;
  }

  free_string(&num_str);

  return result;
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
  (void) pf;
  (void) num;
  return NULL;
}