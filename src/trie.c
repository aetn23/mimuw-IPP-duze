#include <stdlib.h>
#include <string.h>

#include "trie.h"

Trie *init_trie(char prefix, char *number, size_t number_size) {
  Trie *trie;

  trie = malloc(sizeof(Trie));
  trie->children = NULL;
  init_string(&trie->forward_number, number_size + 1);
  transfer_chars_to_string(&trie->forward_number, number, number_size);
  trie->number = prefix;

  return trie;
}

void free_trie(Trie *trie) {
  if (trie == NULL)
    return;

  TrieNodeList *to_free = trie->children;
  free_string(&trie->forward_number);
  free(trie);

  while (to_free != NULL) {
    TrieNodeList *to_free_next = to_free->next;
    free_trie(to_free->node);
    free(to_free);
    to_free = to_free_next;
  }
}