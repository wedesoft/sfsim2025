#include "world.h"


void *scale_world(void *world_, double scale) {
  world_t *world = world_;
  world_t *result = make_world();
  result->bodies = world->bodies;
  result->joints = world->joints;
  return result;
}
