#ifndef _TRIE_
#define _TRIE_

#include "my_string.h"
#include "lists.h"

// todo this should be in diffrenet file
typedef struct StringTuple {
  String first, second;
} StringTuple;

typedef struct Trie Trie;

struct Trie {
  StringTuple *phone_forward;
  CharList *children;
};



#endif //_TRIE_