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

void *world_change(double time, double dt, void *world_, void *data_) {
  world_t *world = world_;
  world_t *result = make_world();
  world_info_t *data = data_;
  vector_t p[world->states.size]; memset(p, 0, sizeof(p));
  vector_t t[world->states.size]; memset(t, 0, sizeof(t));
  for (int c=0; c<data->iterations; c++)
    for (int k=0; k<data->joints.size; k++) {
      joint_t joint = get_joint(data->joints)[k];
      state_t *s1 = get_pointer(world->states)[joint.i];
      state_t *s2 = get_pointer(world->states)[joint.j];
      body_t body1 = get_body(data->bodies)[joint.i];
      body_t body2 = get_body(data->bodies)[joint.j];
      forces_t forces1 = get_forces(data->forces)[joint.i];
      forces_t forces2 = get_forces(data->forces)[joint.j];
      state_t *prediction1 = predict(s1, body1, forces1, p[joint.i], t[joint.i], dt);
      state_t *prediction2 = predict(s2, body2, forces2, p[joint.j], t[joint.j], dt);
      large_matrix_t j = ball_in_socket_jacobian(s1, s2, joint);
      large_vector_t b = ball_in_socket_correction(s1, s2, joint);
      vector_t p1; vector_t p2; vector_t t1; vector_t t2;
      correcting_impulse(body1, body2, prediction1, prediction2, j, b, &p1, &p2, &t1, &t2);
      p[joint.i] = vector_add(p[joint.i], p1);
      p[joint.j] = vector_add(p[joint.j], p2);
      t[joint.i] = vector_add(t[joint.i], t1);
      t[joint.j] = vector_add(t[joint.j], t2);
    };
  for (int i=0; i<world->states.size; i++) {
    state_t *s = get_pointer(world->states)[i];
    body_t b = get_body(data->bodies)[i];
    forces_t f = get_forces(data->forces)[i];
    vector_t position_change = vector_scale(s->speed, dt);
    vector_t speed_change_ = speed_change(f, b, p[i], dt);
    quaternion_t orientation_change = quaternion_product(vector_to_quaternion(vector_scale(s->rotation, 0.5 * dt)), s->orientation);
    vector_t rotation_change_ = rotation_change(s, f, b, t[i], dt);
    append_pointer(&result->states, state(position_change, speed_change_, orientation_change, rotation_change_));
  };
  return result;
}
