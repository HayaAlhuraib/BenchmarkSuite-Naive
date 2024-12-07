#ifndef __INCLUDE_TYPES_H_
#define __INCLUDE_TYPES_H_

#include <stddef.h>  // For size_t

// Define the argument structure
typedef struct {
  byte*   input;
  byte*   output;

  size_t size;

  int     cpu;
  int     nthreads;

} args_t;

#endif // __INCLUDE_TYPES_H_
