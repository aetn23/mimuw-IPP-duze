#ifndef _STRING_LIST_
#define _STRING_LIST_

#include "my_string.h"

typedef struct Trie Trie;

typedef struct TrieNodeList {
  Trie *nodes;
  size_t size;
} TrieNodeList;

void init_trie_node_list(TrieNodeList *list);

void add_trie_to_node_list (TrieNodeList *list, Trie *node);

#endif //_STRING_LIST_