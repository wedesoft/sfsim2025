#include <gc.h>
#include <stdio.h>
#include <string.h>
#include "cache.h"


image_t cache_image(cache_t *cache, const char *format, int level, int y, int x) {
  char *path = GC_MALLOC_ATOMIC(256);
  snprintf(path, 256, format, level, x, y);
  int found = -1;
  for (int i=0; i<cache->values.size; i++) {
    if (!strcmp(path, get_pointer(cache->keys)[i])) {
      found = i;
      break;
    };
  };
  image_t image;
  if (found >= 0) {
    image = get_image(cache->values)[found];
    remove_pointer(&cache->keys, found);
    remove_image(&cache->values, found);
  } else {
    image = read_image(path);
    if (cache->values.size >= cache->size) {
      remove_pointer(&cache->keys, 0);
      remove_image(&cache->values, 0);
    };
  };
  append_pointer(&cache->keys, path);
  append_image(&cache->values, image);
  return image;
}
