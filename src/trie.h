#ifndef _TRIE_
#define _TRIE_

#include "my_string.h"
#include "lists.h"

// todo this should be in different file
typedef struct StringTuple {
  String first, second;
} StringTuple;

struct Trie {
  char number;
  String forward_number;
  TrieNodeList *children;
};

Trie *init_trie(char prefix, char *number, size_t number_size);

void free_trie(Trie *trie);

#endif //_TRIE_