#include "str.h"
#include <string.h>

string span(char *beg, char *end) {
  string r = {0};
  r.str = beg;
  r.len = beg ? end - beg : 0;
  return r;
}

string trimleft(string s) {
  if (!s.str)
    return s;

  while (s.len > 0 && (*s.str == ' ' || *s.str == '\t' || *s.str == '\n')) {
    s.str++;
    s.len--;
  }

  return s;
}

string trimright(string s) {
  while (s.len > 0 && (s.str[s.len - 1] == ' ' || s.str[s.len - 1] == '\t' ||
                       s.str[s.len - 1] == '\n')) {
    s.len--;
  }
  return s;
}

string substr(string s, ptrdiff_t start) {
  if (start < 0) {
    start = 0;
  }

  if (start >= s.len) {
    return (string){NULL, 0};
  }

  s.str += start;
  s.len -= start;

  return s;
}

int str_equal(string a, string b) {
  return a.len == b.len && (!a.len || !memcmp(a.str, b.str, a.len));
}

int str_startswith(string s, string prefix) {
  return s.len >= prefix.len &&
         (!prefix.len || !memcmp(s.str, prefix.str, prefix.len));
}

int str_endswith(string s, string suffix) {
  return s.len >= suffix.len &&
         (!suffix.len ||
          !memcmp(s.str + s.len - suffix.len, suffix.str, suffix.len));
}

int str_contains(string s, string substr) {
  if (substr.len == 0)
    return 1;
  if (s.len < substr.len)
    return 0;

  if (substr.len == 1) {
    return memchr(s.str, substr.str[0], s.len) != NULL;
  }

  char first = substr.str[0];
  ptrdiff_t last_pos = s.len - substr.len;

  for (ptrdiff_t i = 0; i <= last_pos; i++) {
    if (s.str[i] == first && !memcmp(s.str + i, substr.str, substr.len))
      return 1;
  }
  return 0;
}

string str_splitonce(string *s, char delim) {
  if (!s->str || s->len == 0)
    return (string){NULL, 0};

  char *p = memchr(s->str, delim, s->len);
  if (!p) {
    string r = *s;
    *s = (string){NULL, 0};
    return r;
  }

  string r = {s->str, p - s->str};
  s->str = p + 1;
  s->len -= r.len + 1;
  return r;
}
