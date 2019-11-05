#include "mechanics.h"
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
