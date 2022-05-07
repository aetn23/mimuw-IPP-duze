/** @file
 * Implementacja modułu drzew Trie.
 * @author Mikołaj Piróg
 * @copyright Mikołaj Piróg
 * @date 2022
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "memory_management.h"
#include "trie.h"

/**
 * Struktura implementująca drzewa Trie.
 */
struct Trie {
  char number;           /**< Prefiks przechowywany w węźle. */
  String forward_number; /**< Przekierowywał numer telefony. */
  Trie **children;       /**< Tablica wskaźników dzieci węzła. */
  Trie *parent;          /**< Wskaźnik na rodzica węzła. */
};

bool init_trie(Trie **trie, const char prefix, Trie *parent) {
  *trie = malloc(sizeof(Trie));
  if (!check_alloc(*trie))
    return false;

  (*trie)->children = calloc(ALPHABET_SIZE, sizeof(Trie *));
  if (!check_alloc((*trie)->children)) {
    free(*trie);

    return false;
  }

  if (!init_string(&(*trie)->forward_number, 0)) {
    free(*trie);
    free((*trie)->children);

    return false;
  }

  (*trie)->number = prefix;
  (*trie)->parent = parent;

  return true;
}

/** @brief Zwraca pierwsze dziecko, nie będące @p NULL.
 * Zwraca pierwsze dziecko, nie będące @p NULL. Jeśli takiego nie ma, zwraca
 * NULL. Zakłada poprawność parametrów.
 * @param[in] node - Wskaźnik na węzeł Trie;
 * @return Wskaźnik na pierwsze dziecko nie będące @p NULL, lub @p NULL, jeśli
 * takie nie istnieje.
 */
Trie *get_first_non_null_child(const Trie *node) {
  for (size_t i = 0; i < ALPHABET_SIZE; i++) {
    if (node->children[i] != NULL)
      return node->children[i];
  }

  return NULL;
}

void free_trie(Trie *trie) {
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
      free_string(&current_node->forward_number);
      free(current_node->children);
      free(current_node);

      return;
    }

    next_node->children[number_char_to_int(current_node->number)] = NULL;
    free_string(&current_node->forward_number);
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
Trie *get_child(const Trie *root, const char number) {
  return root->children[number_char_to_int(number)];
}

/** @brief Dodaje dziecko do tablicy dzieci węzła.
 * Dodaje dziecko do tablicy dzieci węzła. Węzeł @p node przejmuje @p child na
 * własność.
 * @param[in] node - wskaźnik na węzeł będący rodzicem;
 * @param[in] child - wskaźnik na węzeł będący dzieckiem;
 */
void add_child_to_trie(Trie *node, Trie *child) {
  node->children[number_char_to_int(child->number)] = child;
}

bool add_value(Trie *root, const String *route, String *value) {
  Trie *current_node = root;
  for (size_t i = 0; i < route->size; i++) {
    Trie *next_node = get_child(current_node, route->content[i]);

    if (next_node == NULL) {
      Trie *potential_next_node;
      if (!init_trie(&potential_next_node, route->content[i], current_node))
        return false;

      add_child_to_trie(current_node, potential_next_node);
      current_node = potential_next_node;
    } else {
      current_node = next_node;
    }
  }
  if (current_node->forward_number.size == 0) {
    current_node->forward_number = *value;
  } else {
    free_string(&current_node->forward_number);
    current_node->forward_number = *value;
  }

  return true;
}

void remove_subtree(Trie **root, char const *route_to_subtree) {
  Trie *current_node = *root;
  Trie *previous_node = NULL;

  for (size_t i = 0; route_to_subtree[i] != NULL_CHAR; i++) {
    if (!isdigit(route_to_subtree[i]))
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
    free_trie(current_node);
  } else {
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
      if (current_node->children[i] != NULL)
        current_node->children[i]->parent = NULL;

      free_trie(current_node->children[i]);
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