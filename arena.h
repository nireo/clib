#ifndef __CLIB_ARENA_H__
#define __CLIB_ARENA_H__
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// allocate `n` objects of type `t` from arena `a`.
// aborts if out of memory.
#define new(a, t, n) (t *)alloc(a, sizeof(t), _Alignof(t), n)

// a simple arena allocator.
typedef struct {
  char *beg;
  char *end;
} arena;

// allocate `count` objects of `size` bytes with `align` alignment from arena
// `a`.
static inline void *alloc(arena *a, ptrdiff_t size, ptrdiff_t align,
                          ptrdiff_t count) {
  ptrdiff_t padding = -(uintptr_t)a->beg & (align - 1);
  ptrdiff_t available = a->end - a->beg - padding;
  if (available < 0 || count > available / size) {
    abort();
  }
  void *p = a->beg + padding;
  a->beg += padding + count * size;
  return memset(p, 0, count * size);
}

// create a new arena with capacity `cap` bytes.
static inline arena new_arena(ptrdiff_t cap) {
  arena a = {0};
  a.beg = (char *)malloc(cap);
  a.end = a.beg ? a.beg + cap : 0;
  return a;
}

#endif
