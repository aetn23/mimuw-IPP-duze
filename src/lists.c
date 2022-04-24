#include "lists.h"
#include "memory_management.h"
#include "trie.h"

void init_trie_node_list(TrieNodeList *list) {
  list->size = 0;
  list->nodes = NULL;
}

void add_trie_to_node_list(TrieNodeList *list, Trie *node) {
  if (list->size == 0) {
    list->nodes = malloc_wrapper(sizeof(Trie));
    list->nodes[0] = *node;
    list->size++;
    return;
  }

  list->nodes = realloc_wrapper(list->nodes, (list->size + 1) * sizeof(Trie));
  list->nodes[list->size] = *node;
  list->size++;
}
