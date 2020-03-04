#include <gc.h>
#include <stdio.h>
#include "cache.h"


image_t cache_image(lru_cache_t *cache, const char *format, int level, int y, int x) {
  char *path = GC_MALLOC_ATOMIC(256);
  snprintf(path, 256, format, level, x, y);
  image_t image = read_image(path);
  append_image(&cache->list, image);
  return image;
}
