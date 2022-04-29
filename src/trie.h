#ifndef _TRIE_
#define _TRIE_

#define ALPHABET_SIZE 10

#include "my_string.h"

typedef struct Trie Trie;

struct Trie {
  char number;
  String forward_number;
  Trie **children;
};

bool init_trie(Trie **trie, char prefix, char *number, size_t number_size);

void free_trie(Trie *trie);

void add_value(Trie *root, String *prefix, String *value);

void remove_subtree (Trie **root, String *value_to_remove);

bool get_deepest_non_null_string_in_trie (Trie *root, String *path);

#endif //_TRIE_