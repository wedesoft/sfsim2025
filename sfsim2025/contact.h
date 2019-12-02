#pragma once
#include <stdbool.h>
#include "large_matrix.h"
#include "state.h"
#include "vector.h"


typedef struct {
  int i;
  int j;
  vector_t normal;
  vector_t point;
  double distance;
  double restitution;
  double normal_speed;
  double friction;
} contact_t;

vector_t relative_speed(state_t *state1, state_t *state2, vector_t point);

// Initialize contact.
static inline contact_t contact(int i, int j, vector_t normal, vector_t point, double distance, double restitution,
                                double normal_speed, double friction) {
  return (contact_t){.i = i, .j = j, .normal = normal, .point = point, .distance = distance, .restitution=restitution,
                     .normal_speed = normal_speed, .friction=friction};
}

large_matrix_t contact_jacobian(contact_t contact, state_t *state1, state_t *state2);

large_vector_t contact_correction(contact_t contact, bool do_restitution);

void contact_impulse(body_t body1, body_t body2, state_t *state1, state_t *state2, contact_t contact,
                     vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2, bool do_restitution);

// Check whether this is a resting or a colliding contact.
static inline bool is_resting(contact_t contact, double threshold) {
  return contact.normal_speed * contact.restitution > -threshold;
}
