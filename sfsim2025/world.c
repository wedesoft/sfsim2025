#include <assert.h>
#include "mechanics.h"
#include "state.h"
#include "world.h"


void *scale_world(void *world_, double scale) {
  world_t *world = world_;
  world_t *result = make_world();
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
  assert(a->states.size == b->states.size);
  for (int i=0; i<a->states.size; i++) {
    state_t *s1 = get_pointer(a->states)[i];
    state_t *s2 = get_pointer(b->states)[i];
    append_pointer(&result->states, add_states(s1, s2));
  };
  return result;
}

void *world_change(double t, double dt, void *world_, void *data_) {
  world_t *world = world_;
  world_t *result = make_world();
  world_info_t *data = data_;
  for (int i=0; i<world->states.size; i++) {
    state_t *s = get_pointer(world->states)[i];
    body_t b = get_body(data->bodies)[i];
    forces_t f = get_forces(data->forces)[i];
    vector_t position_change = vector_scale(s->speed, dt);
    vector_t speed_change = vector_scale(f.force, dt / b.mass);
    quaternion_t orientation_change = quaternion_product(vector_to_quaternion(vector_scale(s->rotation, 0.5 * dt)), s->orientation);
    matrix_t inertia = rotate_matrix(s->orientation, b.inertia);
    vector_t coriolis = vector_negative(cross_product(s->rotation, matrix_vector_dot(inertia, s->rotation)));
    vector_t rotation_change = vector_scale(matrix_vector_dot(inverse(inertia), vector_add(f.torque, coriolis)), dt);
    append_pointer(&result->states, state(position_change, speed_change, orientation_change, rotation_change));
  };
  return result;
}
