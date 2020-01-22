#pragma once
#include <tgmath.h>
#include <stdbool.h>
#include "large_matrix.h"
#include "state.h"
#include "vector.h"


typedef struct {
  int i;
  int j;
  double friction;
} contact_candidate_t;

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

// Initialize contact candidate.
static inline contact_candidate_t contact_candidate(int i, int j, double friction) {
  return (contact_candidate_t){.i = i, .j = j, .friction=friction};
}

// Initialize contact.
static inline contact_t contact(int i, int j, vector_t normal, vector_t point, double distance, double restitution,
                                double normal_speed, double friction) {
  return (contact_t){.i = i, .j = j, .normal = normal, .point = point, .distance = distance, .restitution=restitution,
                     .normal_speed = normal_speed, .friction=friction};
}

large_matrix_t contact_jacobian(contact_t contact, state_t *state1, state_t *state2);

large_vector_t contact_correction(contact_t contact, bool do_restitution);

void friction_tangentials(quaternion_t orientation, vector_t normal, vector_t *t1, vector_t *t2);

large_matrix_t friction_jacobian(contact_t contact, state_t *state1, state_t *state2);

large_vector_t friction_correction(contact_t contact);

void contact_impulse(body_t body1, body_t body2, state_t *state1, state_t *state2, contact_t contact, bool do_restitution,
                     vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2, double *lambda_);

void friction_impulse(body_t body1, body_t body2, state_t *state1, state_t *state2, contact_t contact,
                      vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2, double contact_lambda);

// Check whether this is a resting or a colliding contact.
static inline bool is_resting(contact_t contact, double threshold) {
  return fabs(contact.normal_speed * contact.restitution) < threshold;
}
