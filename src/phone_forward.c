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
  Trie *root;              /**< Wskaźnik na korzeń drzewa Trie. */
  Trie *reverse_trie_root; /**< Wskaźnik na korzeń drzewa Trie, przechowującego
                                odwrotne przekierowania. */
};

/**
 * Implementacja struktury przechowującej numery telefonów.
 */
struct PhoneNumbers {
  String *numbers_sequence; /**< Tablica struktur reprezentujących napisy. */
  size_t size; /**< Liczba elementów w tablicy @p numbers_sequence. */
  size_t allocated_size; /**< Liczba elementów, na jakie @p numbers_sequence
                               ma miejsce w pamięci. */
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
    if (numbers->numbers_sequence[i].size != 0)
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
    free_trie(trie, false);

    return NULL;
  }

  result->root = trie;
  result->reverse_trie_root = reverse_trie;

  return result;
}

void phfwdDelete(PhoneForward *pf) {
  if (pf == NULL)
    return;

  free_trie(pf->reverse_trie_root, false);
  free_trie(pf->root, false);
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

  Trie *ptr_to_reverse_trie_node;
  PhoneNumbersPtrSizeTPair *reverse_trie_result =
          add_value_reverse_trie(pf->reverse_trie_root, &num2_string,
                                 &num1_string, &ptr_to_reverse_trie_node);
  if (!check_alloc(reverse_trie_result)) {
    free_string(&num2_string);
    return false;
  }

  Trie *trie_result = add_value_normal_trie(pf->root, &num1_string,
                                            &num2_string, reverse_trie_result);
  if (!check_alloc(trie_result)) {
    free_string(&num2_string);
    detach_node_from_trie(ptr_to_reverse_trie_node);
    free_trie(ptr_to_reverse_trie_node, false);
    free_pair_struct(reverse_trie_result);

    return false;
  }


  return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
  if (num != NULL && pf != NULL)
    remove_subtree(&pf->root, num);
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
  if (pnum == NULL || idx >= pnum->size || pnum->numbers_sequence->size == 0)
    return NULL;

  return pnum->numbers_sequence[idx].content;
}

String *phnum_get_string(PhoneNumbers const *pnum, size_t idx) {
  if (pnum == NULL || idx >= pnum->size)
    return NULL;

  while (pnum->numbers_sequence[idx].size == 0) {
    idx++;

    if (idx >= pnum->size)
      return NULL;
  }

  return &pnum->numbers_sequence[idx];
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

/** @brief Usuwa duplikaty z @p pnum.
 * Usuwa duplikaty z @p pnum. Zwalnia pamięć znalezionego duplikatu i w jego
 * miejsce wstawia strukturę @p String zainicjalizowaną zerami.
 * @param[in] pnum - Wskaźnik na strukturę @p PhoneNumbers.
 */
static void remove_repetitions(PhoneNumbers *pnum) {
  for (size_t i = 0; i < pnum->size - 1; i++) {
    if (pnum->numbers_sequence[i].size == 0)
      return;

    if (strcmp(pnum->numbers_sequence[i].content,
               pnum->numbers_sequence[i + 1].content) == 0) {
      free_string(&pnum->numbers_sequence[i]);
      init_string(&pnum->numbers_sequence[i], 0);
    }
  }
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
  if (pf == NULL)
    return NULL;

  if (num == NULL)
    return phnumNew(0);

  String num_str;
  if (!init_string(&num_str, START_ARRAY_SIZE_SMALL)) {
    return phnumNew(0);
  }

  bool memory_failure = false;
  if (!parse_chars_to_string_wrapper(num, &num_str, &memory_failure)) {
    if (!memory_failure) {
      free_string(&num_str);

      return phnumNew(0);
    }

    free_string(&num_str);

    return NULL;
  }

  if (is_empty_string(&num_str)) {
    free_string(&num_str);

    return phnumNew(0);
  }

  PhoneNumbers *result = get_reversed_numbers(pf->reverse_trie_root, &num_str);
  if (result == NULL) {
    free_string(&num_str);

    return NULL;
  }

  if (!push_back_numbers(result, &num_str)) {
    free_string(&num_str);
    phnumDelete(result);

    return NULL;
  }


  qsort(result->numbers_sequence, result->size, sizeof(String),
        &phone_num_compare);

  remove_repetitions(result);

  qsort(result->numbers_sequence, result->size, sizeof(String),
        &phone_num_compare);

  return result;
}

size_t pnum_size(const PhoneNumbers *pnum) { return pnum->size; }

PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num) {
  if (pf == NULL)
    return NULL;

  if (num == NULL)
    return phnumNew(0);

  PhoneNumbers *reverse_result = phfwdReverse(pf, num);
  if (reverse_result == NULL)
    return NULL;

  if (reverse_result->size == 0) {
    phnumDelete(reverse_result);

    return phnumNew(0);
  }


  PhoneNumbers *result = phnumNew(START_ARRAY_SIZE_SMALL);
  if (!check_alloc(result)) {
    phnumDelete(reverse_result);

    return NULL;
  }

  for (size_t i = 0; i < reverse_result->size; i++) {
    if (reverse_result->numbers_sequence[i].content == NULL)
      break;

    PhoneNumbers *forwarded_number =
            phfwdGet(pf, reverse_result->numbers_sequence[i].content);
    if (!check_alloc(forwarded_number)) {
      phnumDelete(reverse_result);
      phnumDelete(result);

      return NULL;
    }

    if (strcmp(num, forwarded_number->numbers_sequence[0].content) == 0) {
      if (!push_back_numbers(result, &reverse_result->numbers_sequence[i])) {
        phnumDelete(reverse_result);
        phnumDelete(result);
        phnumDelete(forwarded_number);

        return NULL;
      }

      reverse_result->numbers_sequence[i].content = NULL;
      reverse_result->numbers_sequence[i].size = 0;
      reverse_result->numbers_sequence[i].allocated_size = 0;
    }

    phnumDelete(forwarded_number);
  }

  phnumDelete(reverse_result);

  return result;
}