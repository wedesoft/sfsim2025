#pragma once
#include "list.h"
#include "image.h"


typedef struct {
  int size;
  list_t keys;
  list_t values;
} cache_t;


static inline cache_t make_image_cache(int size) {
  return (cache_t){.size = size, .keys = make_list(), .values = make_list()};
}

image_t cache_image(cache_t *cache, const char *format, int level, int y, int x);
