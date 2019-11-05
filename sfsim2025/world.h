#pragma once
#include <gc.h>
#include "list.h"


typedef struct {
  list_t bodies;
} world_t;


world_t *make_world(void) {
  world_t *result = GC_MALLOC(sizeof(world_t));
  result->bodies = make_list();
  return result;
}
