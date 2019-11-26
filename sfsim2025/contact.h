#pragma once
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
} contact_t;

// Initialize contact.
static inline contact_t contact(int i, int j, vector_t normal, vector_t point, double distance, double restitution) {
  return (contact_t){.i = i, .j = j, .normal = normal, .point = point, .distance = distance, .restitution=restitution};
}

large_matrix_t contact_jacobian(contact_t contact, state_t *state1, state_t *state2);

large_vector_t contact_correction(contact_t contact, state_t *state1, state_t *state2);

void contact_impulse(body_t body1, body_t body2, state_t *state1, state_t *state2, contact_t contact,
                     vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2);
