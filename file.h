#ifndef __CLIB_FILE_H__
#define __CLIB_FILE_H__

#include "arena.h"
#include "str.h"
#include <stdio.h>

// file_read reads the entire contents of the file at `path` into a buffer
// allocated from arena `a`. it returns a string struct containing the buffer
// and its length. if the file cannot be read, it returns a string with a null
// pointer and length 0.
static inline string file_read(char *path, arena *a) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    return (string){0};
  }

  // Get file size
  if (fseek(f, 0, SEEK_END) != 0) {
    fclose(f);
    return (string){0};
  }

  long size = ftell(f);
  if (size < 0) {
    fclose(f);
    return (string){0};
  }

  if (fseek(f, 0, SEEK_SET) != 0) {
    fclose(f);
    return (string){0};
  }

  // Allocate buffer from arena
  char *buf = new (a, char, size);

  // Read entire file
  size_t nread = fread(buf, 1, size, f);
  fclose(f);

  if ((long)nread != size) {
    return (string){0};
  }

  return (string){buf, size};
}

// file_write writes the contents of string `s` to the file at `path`. it
// returns 1 on success and 0 on failure.
static inline int file_write(char *path, string s) {
  FILE *f = fopen(path, "wb");
  if (!f) {
    return 0;
  }
  size_t nwritten = fwrite(s.str, 1, s.len, f);
  fclose(f);
  return nwritten == (size_t)s.len;
}

static inline int file_append(char *path, string s) {
  FILE *f = fopen(path, "ab");
  if (!f) {
    return 0;
  }
  size_t nwritten = fwrite(s.str, 1, s.len, f);
  fclose(f);
  return nwritten == (size_t)s.len;
}

// file_exists returns 1 if the file at `path` exists and is readable, 0
static inline int file_exists(char *path) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    return 0;
  }
  fclose(f);
  return 1;
}

typedef int (*file_line_callback)(string line, void *context);

static inline int file_readlines(char *path, arena *a,
                                 file_line_callback callback, void *context) {
  string content = file_read(path, a);
  if (content.str == 0) {
    return 0;
  }

  while (content.len > 0) {
    string line = str_splitonce(&content, '\n');

    if (line.len > 0 && line.str[line.len - 1] == '\r') {
      line.len--;
    }

    int ret = callback(line, context);
    if (ret != 0) {
      break;
    }
  }

  return 1;
}

#endif
