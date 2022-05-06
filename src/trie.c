#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "memory_management.h"
#include "trie.h"

struct Trie {
  char number;
  String forward_number;
  Trie **children;
  Trie *parent;
};

//todo inquire possibility of removal of double pointer
bool init_trie(Trie **trie, char prefix, Trie *parent) {
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

  // In this case this operation will never fail, because memory for number has
  // been allocated in string initialization.

  (*trie)->number = prefix;
  (*trie)->parent = parent;

  return true;
}
Trie *get_first_non_null_child(Trie *root) {
  for (size_t i = 0; i < ALPHABET_SIZE; i++) {
    if (root->children[i] != NULL)
      return root->children[i];
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

Trie *get_child(Trie *root, const char prefix) {
  return root->children[number_char_to_int(prefix)];
}

void add_child_to_trie(Trie *root, Trie *child) {
  root->children[number_char_to_int(child->number)] = child;
}

//todo error checking
bool add_value(Trie *root, String *route, String *value) {
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

//Think if this can be done more nicely
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

//todo rename
bool get_deepest_nonempty_value(Trie *root, String *route, String *result) {
  String *potential_value = NULL;
  size_t potential_value_depth = 0;
  Trie *current_node = root;

  for (size_t i = 0; i < route->size; i++) {
    current_node = get_child(current_node, route->content[i]);

    if (current_node == NULL) {
      break;
    }

    if (current_node->forward_number.size != 0) {
      potential_value = &current_node->forward_number;
      potential_value_depth = i + 1;
    }
  }

  if (potential_value == NULL) {
    if (!concatate_from_to(result, route, 0, route->size - 1, result))
      return false;
    return true;
  }

  if (!concatate_from_to(potential_value, route, potential_value_depth, route->size - 1, result))
    return false;
  return true;
}
