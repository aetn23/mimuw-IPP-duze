#ifndef _TRIE_
#define _TRIE_

#define ALPHABET_SIZE 10
#define FIRST_ALPHABET_LETTER '0'
#define LAST_ALPHABET_LETTER '9'

#include "my_string.h"

/** Typ reprezentujący drzewa Trie.
 *
 */
typedef struct Trie Trie;


bool init_trie(Trie **trie, char prefix, Trie *parent);

void free_trie(Trie *trie);

bool add_value(Trie *root, String *route, String *value);

void remove_subtree(Trie **root, char const *route_to_subtree);

bool get_deepest_nonempty_value(Trie *root, String *route, String *result);

#endif //_TRIE_