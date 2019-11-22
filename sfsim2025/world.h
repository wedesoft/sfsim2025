#pragma once
#include <gc.h>
#include "list.h"


typedef struct {
  list_t states;
} world_t;


static inline world_t *make_world(void) {
  world_t *result = GC_MALLOC(sizeof(world_t));
  result->states = make_list();
  return result;
}

void *scale_world(void *world_, double scale);

void *add_worlds(void *a_, void *b_);

typedef struct {
  int iterations;
  list_t bodies;
  list_t joints;
  list_t forces;
} world_info_t;

static inline world_info_t make_world_info(void) {
  world_info_t result;
  result.iterations = 1;
  result.bodies = make_list();
  result.joints = make_list();
  result.forces = make_list();
  return result;
}

state_t *state_change(state_t *s, body_t b, forces_t f, vector_t p, vector_t t, double dt);

void *world_change(double time, double dt, void *world_, void *data_);
