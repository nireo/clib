#ifndef __CLIB_STR_H__
#define __CLIB_STR_H__

#include "arena.h"
#include "slice.h"
#include <stddef.h>

#define S(s) (str_t){s, sizeof(s) - 1}

// str_t is a string structure that holds a pointer to a character array and its
// length. the character array is not null-terminated.
//
// this is also non-owning, so the user is responsible for managing the memory
// of the character array.
// the utility function mainly focus on writing parsers so those are most
// focused on that with some missing features.
typedef struct {
  char *str;
  ptrdiff_t len;
} string;

string str_trimleft(string s);
string str_trimright(string s);
string str_substr(string s, ptrdiff_t start);
string str_span(char *beg, char *end);
int str_equal(string a, string b);
int str_startswith(string s, string prefix);
int str_endswith(string s, string suffix);
int str_contains(string s, string substr); // this is just a linear search
string str_splitonce(string *s, char delim);

#endif
