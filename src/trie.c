/** @file
 * Implementacja modułu drzew Trie.
 * @author Mikołaj Piróg
 * @copyright Mikołaj Piróg
 * @date 2022
 */

#include <stdlib.h>

#include "memory_management.h"
#include "phone_forward.h"
#include "trie.h"

/**
 * Struktura implementująca drzewa Trie.
 */
struct Trie {
  char number;          /**< Prefiks przechowywany w węźle. */
  bool is_reverse_trie; /**< Wartość @p bool, informująca o typie
                           przechowywanych wartości. **/
  union {    /**< Anonimowa unia przechowująca odpowiednie type wartości. **/
    struct { /**< Anonimowa struktura przechowująca wartości drzewa
                przekierowań. **/
      String forward_number; /**< Przekierowywany numer telefony. **/
      PhoneNumbersPtrSizeTPair
              *ptr_to_node_in_reverse_trie; /**< Wskaźnik na parę przechowującą
                                               informację o lokalizacji danej o
                                               przekierowaniu w drzewie
                                               odwrotnych przekierowań. **/
    };
    PhoneNumbers *reverse_trie_phone_numbers; /**< Wskaźnik na strukturę @p
                                                 PhoneNumbers przechowującą
                                                 odwrotne przekierowania. **/
  };
  Trie **children; /**< Tablica wskaźników dzieci węzła. */
  Trie *parent;    /**< Wskaźnik na rodzica węzła. */
};

bool init_trie(Trie **trie, const char prefix, Trie *parent, bool is_reverse) {
  *trie = malloc(sizeof(Trie));
  if (!check_alloc(*trie))
    return false;

  (*trie)->children = calloc(ALPHABET_SIZE, sizeof(Trie *));
  if (!check_alloc((*trie)->children)) {
    free(*trie);

    return false;
  }

  if (is_reverse) {
    (*trie)->reverse_trie_phone_numbers = phnumNew(START_ARRAY_SIZE_SMALL);
    if (!check_alloc((*trie)->reverse_trie_phone_numbers)) {
      free((*trie)->children);
      free(*trie);

      return false;
    }
  } else {
    if (!init_string(&(*trie)->forward_number, 0)) {
      free((*trie)->children);// Order of this was reversed which most likely
                              // was a bug
      free(*trie);

      return false;
    }
  }

  (*trie)->number = prefix;
  (*trie)->parent = parent;
  if (is_reverse) {
    (*trie)->is_reverse_trie = true;
  } else {
    (*trie)->ptr_to_node_in_reverse_trie = NULL;
    (*trie)->is_reverse_trie = false;
  }

  return true;
}

/** @brief Zwraca pierwsze dziecko, nie będące @p NULL.
 * Zwraca pierwsze dziecko, nie będące @p NULL. Jeśli takiego nie ma, zwraca
 * NULL. Zakłada poprawność parametrów.
 * @param[in] node - Wskaźnik na węzeł Trie;
 * @return Wskaźnik na pierwsze dziecko nie będące @p NULL, lub @p NULL, jeśli
 * takie nie istnieje.
 */
static Trie *get_first_non_null_child(const Trie *node) {
  for (size_t i = 0; i < ALPHABET_SIZE; i++) {
    if (node->children[i] != NULL)
      return node->children[i];
  }

  return NULL;
}

void free_trie(Trie *trie, bool update_reverse) {
  if (trie == NULL)
    return;

  Trie *current_node = trie;
  while (true) {
    Trie *potential_next_node = get_first_non_null_child(current_node);

    if (potential_next_node != NULL) {
      current_node = potential_next_node;
      continue;
    }

    Trie *next_node = current_node->parent;
    if (next_node == NULL) {
      if (current_node->is_reverse_trie) {
        phnumDelete(current_node->reverse_trie_phone_numbers);
      } else if (update_reverse && current_node->forward_number.size != 0) {
        free_string(phnum_get_string(
                current_node->ptr_to_node_in_reverse_trie->ptr,
                current_node->ptr_to_node_in_reverse_trie->number));
        free_pair_struct(current_node->ptr_to_node_in_reverse_trie);
        free_string(&current_node->forward_number);
      } else {
        free_pair_struct(current_node->ptr_to_node_in_reverse_trie);
        free_string(&current_node->forward_number);
      }

      free(current_node->children);
      free(current_node);

      return;
    }

    next_node->children[number_char_to_int(current_node->number)] = NULL;
    if (current_node->is_reverse_trie) {
      phnumDelete(current_node->reverse_trie_phone_numbers);
    } else if (update_reverse && current_node->forward_number.size != 0) {
      free_string(phnum_get_string(
              current_node->ptr_to_node_in_reverse_trie->ptr,
              current_node->ptr_to_node_in_reverse_trie->number));
      free_pair_struct(current_node->ptr_to_node_in_reverse_trie);
      free_string(&current_node->forward_number);
    } else {
      free_pair_struct(current_node->ptr_to_node_in_reverse_trie);
      free_string(&current_node->forward_number);
    }

    free(current_node->children);
    free(current_node);
    current_node = next_node;
  }
}

/** @brief Zwraca wskaźnik na dziecko o zadanym numerze.
 * Zwraca wskaźnik na dziecko o zadanym numerze.
 * @param[in] root - wskaźnik na węzeł Trie;
 * @param[in] number - znak szukanego dziecka;
 * @return Wskaźnik na dziecko o znaku równym @p number.
 */
static Trie *get_child(const Trie *root, const char number) {
  return root->children[number_char_to_int(number)];
}

/** @brief Dodaje dziecko do tablicy dzieci węzła.
 * Dodaje dziecko do tablicy dzieci węzła. Węzeł @p node przejmuje @p child na
 * własność.
 * @param[in] node - wskaźnik na węzeł będący rodzicem;
 * @param[in] child - wskaźnik na węzeł będący dzieckiem;
 */
static void add_child_to_trie(Trie *node, Trie *child) {
  node->children[number_char_to_int(child->number)] = child;
}

/** @brief Dodaje pusty węzeł.
 * Dodaje pusty węzeł, podążając ścieżką @p route.
 * @param[in, out] root - wskaźnik na korzeń drzewa;
 * @param[in] route - napis reprezentujący drogę do dodawanego węzła.
 * @return Jeśli alokację pamięci powiodą się, zwraca wskaźnik na ostatni dodany
 * węzeł. W przeciwnym razie, @p NULL.
 */
static Trie *add_value_common_part(Trie *root, const String *route) {
  Trie *current_node = root;
  for (size_t i = 0; i < route->size; i++) {
    Trie *next_node = get_child(current_node, route->content[i]);

    if (next_node == NULL) {
      Trie *potential_next_node;
      if (!init_trie(&potential_next_node, route->content[i], current_node,
                     current_node->is_reverse_trie))
        return NULL;

      add_child_to_trie(current_node, potential_next_node);
      current_node = potential_next_node;
    } else {
      current_node = next_node;
    }
  }

  return current_node;
}

Trie *
add_value_normal_trie(Trie *root, const String *route, String *value,
                      PhoneNumbersPtrSizeTPair *ptr_to_reverse_trie_value) {
  Trie *node_to_add = add_value_common_part(root, route);
  if (node_to_add == NULL) {
    return NULL;
  }


  if (node_to_add->forward_number.size == 0) {
    node_to_add->forward_number = *value;
  } else {
    free_string(&node_to_add->forward_number);
    free_string(
            phnum_get_string(node_to_add->ptr_to_node_in_reverse_trie->ptr,
                             node_to_add->ptr_to_node_in_reverse_trie->number));
    free_pair_struct(node_to_add->ptr_to_node_in_reverse_trie);

    node_to_add->forward_number = *value;
  }

  node_to_add->ptr_to_node_in_reverse_trie = ptr_to_reverse_trie_value;

  return node_to_add;
}

PhoneNumbersPtrSizeTPair *add_value_reverse_trie(Trie *root,
                                                 const String *route,
                                                 String *value,
                                                 Trie **ptr_to_added_node) {
  Trie *node_to_add = add_value_common_part(root, route);
  if (!check_alloc(node_to_add)) {
    *ptr_to_added_node = NULL;
    free_string(value);
    return NULL;
  }

  *ptr_to_added_node = node_to_add;
  if (!push_back_numbers(node_to_add->reverse_trie_phone_numbers, value)) {
    free_string(value);
    return NULL;
  }

  PhoneNumbersPtrSizeTPair *result = init_ptr_size_pair(
          pnum_size(node_to_add->reverse_trie_phone_numbers) - 1,
          node_to_add->reverse_trie_phone_numbers);
  if (!check_alloc(result)) {
    node_to_add->parent->children[number_char_to_int(node_to_add->number)] =
            NULL;
    node_to_add->parent = NULL;
    free_trie(node_to_add, false);

    return NULL;
  }

  return result;
}

void remove_subtree(Trie **root, char const *route_to_subtree) {
  Trie *current_node = *root;
  Trie *previous_node = NULL;

  for (size_t i = 0; route_to_subtree[i] != NULL_CHAR; i++) {
    if (!is_valid_digit(route_to_subtree[i]))
      return;

    Trie *potential_next_node = get_child(current_node, route_to_subtree[i]);

    if (potential_next_node == NULL) {
      return;
    }

    previous_node = current_node;
    current_node = potential_next_node;
  }

  if (previous_node != NULL) {
    previous_node->children[number_char_to_int(current_node->number)] = NULL;
    current_node->parent = NULL;
    free_trie(current_node, true);
  } else {
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
      if (current_node->children[i] != NULL)
        current_node->children[i]->parent = NULL;

      free_trie(current_node->children[i], true);
      current_node->children[i] = NULL;
    }
  }
}

bool get_num_forward_from_trie(Trie *root, const String *num, String *result) {
  String *potential_value = NULL;
  size_t potential_value_depth = 0;
  Trie *current_node = root;

  for (size_t i = 0; i < num->size; i++) {
    current_node = get_child(current_node, num->content[i]);

    if (current_node == NULL) {
      break;
    }

    if (current_node->forward_number.size != 0) {
      potential_value = &current_node->forward_number;
      potential_value_depth = i + 1;
    }
  }

  if (potential_value == NULL) {
    if (!concatenate_from_to(result, num, 0, num->size - 1, result))
      return false;

    return true;
  }

  if (!concatenate_from_to(potential_value, num, potential_value_depth,
                           num->size - 1, result))
    return false;

  return true;
}

PhoneNumbers *get_reversed_numbers(Trie *reverse_trie_root,
                                   const String *route) {
  if (!reverse_trie_root->is_reverse_trie)
    return NULL;

  PhoneNumbers *result = phnumNew(START_ARRAY_SIZE_SMALL);
  if (!check_alloc(result)) {
    return NULL;
  }

  Trie *current_node = reverse_trie_root;
  for (size_t i = 0; i < route->size; i++) {
    current_node = get_child(current_node, route->content[i]);

    if (current_node == NULL)
      break;

    if (current_node->reverse_trie_phone_numbers != NULL) {
      size_t j = 0;
      while (phnum_get_string(current_node->reverse_trie_phone_numbers, j) !=
             NULL) {
        String reverse_number;
        if (!init_string(&reverse_number, START_ARRAY_SIZE_SMALL)) {
          phnumDelete(result);

          return NULL;
        }


        if (!concatenate_from_to(
                    phnum_get_string(current_node->reverse_trie_phone_numbers,
                                     j),
                    route, i + 1, route->size, &reverse_number) ||
            !push_back_numbers(result, &reverse_number)) {
          phnumDelete(result);
          free_string(&reverse_number);

          return NULL;
        }

        j++;
      }
    }
  }

  return result;
}

void detach_node_from_trie(Trie *node) {
  node->parent->children[number_char_to_int(node->number)] = NULL;
  node->parent = NULL;
}