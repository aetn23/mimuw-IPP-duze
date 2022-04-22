#ifndef _STRING_LIST_
#define _STRING_LIST_

#include "my_string.h"

typedef struct StringList StringList;

struct StringList {
  String string;
  StringList *next;
};

typedef struct CharList CharList;

struct CharList {
  char c;
  CharList *next;
};

#endif //_STRING_LIST_