#include <stdlib.h>
#include <string.h>

#include "trie.h"
#include "memory_management.h"

bool init_trie(Trie **trie, char prefix, char *number, size_t number_size) {
  *trie = malloc(sizeof(Trie));
  if (!check_alloc(trie))
    return false;

  init_trie_node_list(&(*trie)->children);
  if (!init_string(&(*trie)->forward_number, number_size + 1))
    return false;

  // In this case this operation will never fail, because memory for number has
  // been allocated in string initialization.
  transfer_chars_to_string(&(*trie)->forward_number, number, number_size + 1);

  (*trie)->number = prefix;

  return true;
}

void free_trie(Trie *trie) {

  free_string(&trie->forward_number);
  for (size_t i = 0; i < trie->children.size; i++) {
    free_trie(trie->children.nodes[i]);
  }
  free(trie->children.nodes);
  free(trie);
}

Trie *get_child(Trie *root, const char prefix) {
  for (size_t i = 0; i < root->children.size; i++) {
    if (root->children.nodes[i]->number == prefix)
      return root->children.nodes[i];
  }

  return NULL;
}
//todo error checking
//todo find out how does this work exactly
void add_value(Trie *root, String *prefix, String *value) {
  Trie *current_node = root;
  for (size_t i = 0; i < prefix->size; i++) {
    Trie *next_node = get_child(current_node, prefix->content[i]);

    if (next_node == NULL) {
      Trie *tmp_node;
      init_trie(&tmp_node, prefix->content[i], EMPTY_STRING, 0);
      add_trie_to_node_list(&current_node->children, tmp_node);
      current_node = tmp_node;
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
}