#pragma once
#include <gc.h>


typedef struct {
} world_t;


world_t *make_world(void) {
  world_t *result = GC_MALLOC(sizeof(world_t));
  return result;
}
