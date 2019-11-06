#include <assert.h>
#include "mechanics.h"
#include "state.h"
#include "world.h"


void *scale_world(void *world_, double scale) {
  world_t *world = world_;
  world_t *result = make_world();
  result->bodies = world->bodies;
  result->joints = world->joints;
  for (int i=0; i<world->states.size; i++) {
    state_t *state = get_pointer(world->states)[i];
    append_pointer(&result->states, scale_state(state, scale));
  };
  return result;
}

void *add_worlds(void *a_, void *b_) {
  world_t *a = a_;
  world_t *b = b_;
  world_t *result = make_world();
  result->bodies = a->bodies;
  result->joints = a->joints;
  assert(a->states.size == b->states.size);
  for (int i=0; i<a->states.size; i++) {
    state_t *s1 = get_pointer(a->states)[i];
    state_t *s2 = get_pointer(b->states)[i];
    append_pointer(&result->states, add_states(s1, s2));
  };
  return result;
}
