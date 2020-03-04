#pragma once
#include "list.h"
#include "image.h"


typedef struct {
  int size;
  list_t list;
} lru_cache_t;


static inline lru_cache_t make_image_cache(int size) {
  return (lru_cache_t){.size = size, .list = make_list()};
}

image_t cache_image(lru_cache_t *cache, const char *format, int level, int y, int x);
