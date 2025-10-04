#ifndef __CLIB_ARENA_H__
#define __CLIB_ARENA_H__

#include <stddef.h>

#define new(a, t, n) (t *)alloc(a, sizeof(t), _Alignof(t), n)

typedef struct {
  char *beg;
  char *end;
} arena;

void *alloc(arena *a, ptrdiff_t size, ptrdiff_t align, ptrdiff_t count);
arena new_arena(ptrdiff_t cap);

#endif
