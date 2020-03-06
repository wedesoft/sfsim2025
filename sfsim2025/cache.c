#include <gc.h>
#include <stdio.h>
#include <string.h>
#include "cache.h"


static char *format_path(const char *format, int level, int y, int x) {
  char *path = GC_MALLOC_ATOMIC(256);
  snprintf(path, 256, format, level, x, y);
  return path;
}

static int index_of(list_t keys, const char *key) {
  int result = -1;
  for (int i=0; i<keys.size; i++) {
    if (!strcmp(key, get_pointer(keys)[i])) {
      result = i;
      break;
    };
  };
  return result;
}

image_t cache_image(cache_t *cache, const char *format, int level, int y, int x) {
  char *path = format_path(format, level, y, x);
  int found = index_of(cache->keys, path);
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

elevation_t cache_elevation(cache_t *cache, const char *format, int level, int y, int x) {
  char *path = format_path(format, level, y, x);
  int found = index_of(cache->keys, path);
  elevation_t elevation;
  if (found >= 0) {
    elevation = get_elevation(cache->values)[found];
    remove_pointer(&cache->keys, found);
    remove_elevation(&cache->values, found);
  } else {
    elevation = read_elevation(path);
    if (cache->values.size >= cache->size) {
      remove_pointer(&cache->keys, 0);
      remove_image(&cache->values, 0);
    };
  };
  append_pointer(&cache->keys, path);
  append_elevation(&cache->values, elevation);
  return elevation;
}
