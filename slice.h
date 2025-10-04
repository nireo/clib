#ifndef __CLIB_SLICE_H__
#define __CLIB_SLICE_H__

#include "arena.h"
#include <stddef.h>
#include <string.h>

// slice defines a struct that can be used to store a dynamically sized array
// for some type T.
//
// usage:
// typedef slice(int) ints;
// ints numbers = {0};
// *push(&numbers, &a) = 42; // push 42 to numbers using arena a
#define slice(T)                                                               \
  struct {                                                                     \
    T *data;                                                                   \
    ptrdiff_t len;                                                             \
    ptrdiff_t cap;                                                             \
  }

// push appends an element to the slice s, growing it if necessary using arena a
// and returns a pointer to the newly added element. note that the slice s must
// be generated using the slice macro. or look similar to it having a *data
// field, a len field and a cap field.
#define push(s, a)                                                             \
  ((s)->len >= (s)->cap   ? grow(s, sizeof(*(s)->data), a),                    \
   (s)->data + (s)->len++ : (s)->data + (s)->len++)

static inline void grow(void *slice, ptrdiff_t size, arena *a) {
  struct {
    void *data;
    ptrdiff_t len;
    ptrdiff_t cap;
  } replica;

  memcpy(&replica, slice, sizeof(replica));
  replica.cap = replica.cap ? replica.cap : 1;
  ptrdiff_t align = 16;
  void *data = alloc(a, 2 * size, align, replica.cap);
  replica.cap *= 2;

  if (replica.len) {
    memcpy(data, replica.data, size * replica.len);
  }

  replica.data = data;
  memcpy(slice, &replica, sizeof(replica));
}

#endif
