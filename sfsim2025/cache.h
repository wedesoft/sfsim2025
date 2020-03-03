#pragma once
#include <stdio.h>
#include <gc.h>


static inline char *tile_key(int level, int y, int x) {
  char *result = GC_MALLOC_ATOMIC(32);
  snprintf(result, 32, "%d/%d/%d", level, x, y);
  return result;
}
