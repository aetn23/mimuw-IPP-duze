#include <stdlib.h>
#include <string.h>

#include "trie.h"
#include "memory_management.h"

//todo inquire possibility of removal of double pointer
bool init_trie(Trie **trie, char prefix, char *number, size_t number_size) {
  *trie = malloc(sizeof(Trie));
  if (!check_alloc(trie))
    return false;

  (*trie)->children = calloc(ALPHABET_SIZE, sizeof(Trie*));
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
  return root->children[(size_t)(prefix - '0')];
}

void add_child_to_trie(Trie *root, Trie *child) {
  root->children[(int)(child->number - '0')] = child;
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


void remove_subtree (Trie *root, String *value_to_remove) {
  Trie *current_node = root;
  for (size_t i = 0; i < value_to_remove->size; i++) {
    Trie *potential_next_node = get_child(current_node, value_to_remove->content[i]);

    if (potential_next_node == NULL) {
      return;
    } 

    current_node = potential_next_node;
  }

  free_trie(current_node);
}