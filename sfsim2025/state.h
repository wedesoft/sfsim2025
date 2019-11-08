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
static inline vector_t speed_change(forces_t forces, body_t body, vector_t impulse, double dt) {
  return vector_add(vector_scale(impulse, 1 / body.mass), vector_scale(forces.force, dt / body.mass));
}

static inline vector_t rotation_change(state_t *state, forces_t forces, body_t body, vector_t angular, double dt) {
  matrix_t inertia = rotate_matrix(state->orientation, body.inertia);
  vector_t coriolis = vector_negative(cross_product(state->rotation, matrix_vector_dot(inertia, state->rotation)));
  return matrix_vector_dot(inverse(inertia), vector_add(angular, vector_scale(vector_add(forces.torque, coriolis), dt)));
}

void *scale_state(void *s_, double scale);
