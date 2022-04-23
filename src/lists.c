#include "lists.h"
#include "memory_management.h"

void add_trie_to_node_list (TrieNodeList **list, Trie *node) {
  TrieNodeList *new_node = malloc_wrapper(sizeof(TrieNodeList));
  new_node->next = NULL;
  new_node->node = node;

  if (*list == NULL) {
    *list = new_node;
    return;
  }

  TrieNodeList *list_cp = *list;
  while (list_cp->next != NULL) {
    list_cp = list_cp->next;
  }

  (*list)->next = new_node;
}

