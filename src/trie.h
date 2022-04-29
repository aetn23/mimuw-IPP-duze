#ifndef _TRIE_
#define _TRIE_

#include "my_string.h"
#include "lists.h"

struct Trie {
  char number;
  String forward_number;
  TrieNodeList children;
};

bool init_trie(Trie **trie, char prefix, char *number, size_t number_size);

void free_trie(Trie *trie);

void add_value(Trie *root, String *prefix, String *value);

void remove_subtree (Trie *root, String *value_to_remove);

#endif //_TRIE_