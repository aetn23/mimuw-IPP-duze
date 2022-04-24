#include "lists.h"
#include "memory_management.h"
#include "trie.h"

void init_trie_node_list(TrieNodeList *list) {
  list->size = 0;
  list->nodes = NULL;
}

bool add_trie_to_node_list(TrieNodeList *list, Trie *node) {
  if (list->size == 0) {
    list->nodes = malloc(sizeof(Trie));
    if (!check_alloc(list->nodes))
      return false;

    list->nodes[0] = *node;
    list->size++;
    return true;
  }

  Trie *tmp = realloc_wrapper(list->nodes, (list->size + 1) * sizeof(Trie));
  if (!check_alloc(tmp))
    return false;

  list->nodes = tmp;
  list->nodes[list->size] = *node;
  list->size++;

  return true;
}
