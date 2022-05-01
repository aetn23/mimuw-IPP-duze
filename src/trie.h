#ifndef _TRIE_
#define _TRIE_

#define ALPHABET_SIZE 10
#define FIRST_ALPHABET_LETTER '0'
#define LAST_ALPHABET_LETTER '9'

#include "my_string.h"

typedef struct Trie Trie;

struct Trie {
  char number;
  String forward_number;
  Trie **children;
};

bool init_trie(Trie **trie, char prefix);

void free_trie(Trie *trie);

void add_value(Trie *root, String *prefix, String *value);

void remove_subtree(Trie **root, char const *route_to_subtree);

bool get_deepest_nonempty_value(Trie *root, String *route, String *result);

#endif //_TRIE_