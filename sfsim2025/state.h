#pragma once
#include <gc.h>
#include "body.h"
#include "forces.h"
#include "vector.h"
#include "quaternion.h"


typedef struct {
  vector_t position;
  vector_t speed;
  quaternion_t orientation;
  vector_t rotation;
} state_t;

// Create a state vector.
static inline state_t *state(vector_t position, vector_t speed, quaternion_t orientation, vector_t rotation) {
  state_t *result = GC_MALLOC_ATOMIC(sizeof(state_t));
  result->position = position;
  result->speed = speed;
  result->orientation = orientation;
  result->rotation = rotation;
  return result;
}

void *add_states(void *a_, void *b_);

// Compute change of speed given the forces acting on the body.
static inline vector_t speed_change(body_t body, vector_t force, vector_t linear_impulse, double dt) {
  return vector_add(vector_scale(linear_impulse, 1 / body.mass), vector_scale(force, dt / body.mass));
}

static inline vector_t rotation_change(state_t *state, body_t body, vector_t torque, vector_t angular, double dt) {
  matrix_t inertia = rotate_matrix(state->orientation, body.inertia);
  vector_t coriolis = vector_negative(cross_product(state->rotation, matrix_vector_dot(inertia, state->rotation)));
  return matrix_vector_dot(inverse(inertia), vector_add(angular, vector_scale(vector_add(torque, coriolis), dt)));
}

void *scale_state(void *s_, double scale);
