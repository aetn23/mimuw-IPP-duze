#include <stdlib.h>
#include <string.h>

#include "trie.h"

bool init_trie(Trie *trie, char prefix, char *number, size_t number_size) {
  init_trie_node_list(&trie->children);
  if (!init_string(&trie->forward_number, number_size + 1) ||
      !transfer_chars_to_string(&trie->forward_number, number, number_size + 1))
    return false;

  trie->number = prefix;

  return true;
}

void free_trie(Trie *trie) {

  free_string(&trie->forward_number);
  for (size_t i = 0; i < trie->children.size; i++) {
    free_trie(&trie->children.nodes[i]);
  }
  free(trie->children.nodes);
}

Trie *get_child(Trie *root, const char prefix) {
  for (size_t i = 0; i < root->children.size; i++) {
    if (root->children.nodes[i].number == prefix)
      return &root->children.nodes[i];
  }

  return NULL;
}
