#include <stdlib.h>
#include <string.h>

#include "trie.h"

void init_trie(Trie *trie, char prefix, char *number, size_t number_size) {
  init_trie_node_list(&trie->children);
  init_string(&trie->forward_number, number_size + 1);
  transfer_chars_to_string(&trie->forward_number, number, number_size + 1);
  trie->number = prefix;
}

void free_trie(Trie *trie) {

  free_string(&trie->forward_number);
  for (size_t i = 0; i < trie->children.size; i++) {
    free_trie(&trie->children.nodes[i]);
  }
  free(trie->children.nodes);
}