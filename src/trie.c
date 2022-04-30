#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "memory_management.h"
#include "trie.h"

//todo inquire possibility of removal of double pointer
bool init_trie(Trie **trie, char prefix, char *number, size_t number_size) {
  *trie = malloc(sizeof(Trie));
  if (!check_alloc(trie))
    return false;

  (*trie)->children = calloc(ALPHABET_SIZE, sizeof(Trie *));
  if (!init_string(&(*trie)->forward_number, number_size + 1))
    return false;

  // In this case this operation will never fail, because memory for number has
  // been allocated in string initialization.
  transfer_chars_to_string(&(*trie)->forward_number, number, number_size + 1);

  (*trie)->number = prefix;

  return true;
}

//todo rewrite this to avoid recurssion
void free_trie(Trie *trie) {
  if (trie == NULL)
    return;

  free_string(&trie->forward_number);
  for (size_t i = 0; i < ALPHABET_SIZE; i++) {
    free_trie(trie->children[i]);
  }
  free(trie->children);
  free(trie);
}

Trie *get_child(Trie *root, const char prefix) {
  return root->children[(size_t) (prefix - '0')];
}

void add_child_to_trie(Trie *root, Trie *child) {
  root->children[(int) (child->number - '0')] = child;
}

//todo error checking
//todo find out how does this work exactly
void add_value(Trie *root, String *prefix, String *value) {
  Trie *current_node = root;
  for (size_t i = 0; i < prefix->size; i++) {
    Trie *next_node = get_child(current_node, prefix->content[i]);

    if (next_node == NULL) {
      Trie *potential_next_node;
      init_trie(&potential_next_node, prefix->content[i], EMPTY_STRING, 0);
      add_child_to_trie(current_node, potential_next_node);
      current_node = potential_next_node;
    } else {
      current_node = next_node;
    }
  }
  if (current_node->forward_number.size == 0) {
    current_node->forward_number = *value;
  } else {
    //todo this free is unecessary
    free_string(&current_node->forward_number);
    current_node->forward_number = *value;
  }
}

//Think if this can be done more nicely
void remove_subtree(Trie **root, String *value_to_remove) {
  Trie *current_node = *root;
  Trie *previous_node = NULL;
  for (size_t i = 0; i < value_to_remove->size; i++) {
    Trie *potential_next_node = get_child(current_node, value_to_remove->content[i]);

    if (potential_next_node == NULL) {
      return;
    }
    previous_node = current_node;
    current_node = potential_next_node;
  }
  if (previous_node != NULL)
    previous_node->children[(size_t) (current_node->number - '0')] = NULL;
  else
    *root = NULL;
  free_trie(current_node);
}
//todo rename
bool get_deepest_non_null_string_in_trie(Trie *root, String *path, String *result) {
  String *potential_value = NULL;
  size_t potential_value_depth = 0;
  Trie *current_node = root;
  size_t i = 0;

  for (; i < path->size; i++) {
    current_node = get_child(current_node, path->content[i]);

    if (current_node == NULL) {
      break;
    }

    if (!is_empty_string(&current_node->forward_number)) {
      potential_value = &current_node->forward_number;
      potential_value_depth = i + 1;
   }
  }

  if (potential_value == NULL) {
    transfer_chars_to_string(result, path->content, path->size);
    return true;
  }

  if (!concatate_from_to(potential_value, path, potential_value_depth, path->size - 1, result))
    return false;
  return true;
}

/*
bool get_deepest_non_null_string_in_trie_old(Trie *root, String *path, String *result) {
  String *potential_value = NULL;
  size_t potential_value_depth = 0;
  Trie *current_node = root;
  size_t i = 0;

  for (; i <= path->size; i++) {
    if (current_node == NULL) {
      break;
    }

    if (!is_empty_string(&current_node->forward_number)) {
      potential_value = &current_node->forward_number;
      potential_value_depth = i;
    }
    printf("%zu\n", i);
    current_node = get_child(current_node, path->content[i]);
  }

  if (potential_value == NULL) {
    transfer_chars_to_string(result, path->content, path->size);
    return true;
  }

  if (!concatate_from_to(potential_value, path, potential_value_depth, path->size - 1, result))
    return false;
  return true;
}*/