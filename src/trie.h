#ifndef _TRIE_
#define _TRIE_

#include "my_string.h"
#include "lists.h"

struct Trie {
  char number;
  String forward_number;
  TrieNodeList children;
};

void init_trie(Trie *trie, char prefix, char *number, size_t number_size);

void free_trie(Trie *trie);

static inline void test_add(Trie *root, Trie node) {
  Trie *where_to_add = root;
  while (where_to_add->children.nodes != NULL) {
    where_to_add = &where_to_add->children.nodes[0];
  }
  add_trie_to_node_list(&where_to_add->children, &node);
}

#endif //_TRIE_