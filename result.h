#ifndef __CLIB_RESULT_H__
#define __CLIB_RESULT_H__

// this is a silly experiment to see if we can do something like Rust's Result
// and Option types in C. it uses macros to generate the necessary structs and
// functions. I don't think the performance overhead is too large here.

// result
#define result(T)                                                              \
  struct {                                                                     \
    T val;                                                                     \
    char *err;                                                                 \
  }

// create successfl result with value v
#define ok(T, v) ((result(T)){.val = (v), .err = 0})

// create an error with error message e
#define err(T, e) ((result(T)){.val = {0}, .err = (e)})

// check if result is an error
#define is_err(r) ((r).err != 0)

// check if result is ok
#define is_ok(r) ((r).err == 0)

// unwrap the result, aborting if it is an error and printing the error message
#define unwrap(r)                                                              \
  (is_err(r)                                                                   \
       ? (fprintf(stderr, "unwrap failed: %s\n", (r).err), abort(), (r).val)   \
       : (r).val)

// optional type that may or may not contain a value of type T
#define optional(T)                                                            \
  struct {                                                                     \
    T val;                                                                     \
    int has_val;                                                               \
  }

#define some(T, v) ((optional(T)){.val = (v), .has_val = 1})
#define none(T) ((optional(T)){.val = {0}, .has_val = 0})
#define has_value(opt) ((opt).has_val)

// for the unwrap macro to work properly
#include <stdio.h>
#include <stdlib.h>

#endif
