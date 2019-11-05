#pragma once
#include <gc.h>
#include "list.h"


typedef struct {
  list_t bodies;
  list_t joints;
  list_t states;
} world_t;


static inline world_t *make_world(void) {
  world_t *result = GC_MALLOC(sizeof(world_t));
  result->bodies = make_list();
  result->joints = make_list();
  result->states = make_list();
  return result;
}

void *scale_world(void *world_, double scale);
