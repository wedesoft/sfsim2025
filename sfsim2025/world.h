#pragma once
#include <gc.h>
#include "list.h"
#include "hull.h"


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
  double restitution;
  double resting_threshold;
  list_t bodies;
  list_t contact_candidates;
  list_t forces;
  list_t rigid_bodies;
  list_t joints;
} world_info_t;

static inline world_info_t make_world_info(void) {
  world_info_t result;
  result.iterations = 10;
  result.restitution = 0.4;
  result.resting_threshold = 0.8;
  result.bodies = make_list();
  result.contact_candidates = make_list();
  result.forces = make_list();
  result.rigid_bodies = make_list();
  result.joints = make_list();
  return result;
}

state_t *state_change(state_t *s, body_t b, vector_t force, vector_t torque, vector_t linear_impulse, vector_t angular_impulse,
                      double dt);

void *world_change(double time, double dt, void *world_, void *data_);
