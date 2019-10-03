#pragma once
#include <gc.h>
#include "vector.h"
#include "quaternion.h"


typedef struct {
  vector_t position;
  vector_t linear_momentum;
  quaternion_t orientation;
  vector_t angular_momentum;
} state_t;

typedef struct {
  double mass;
  matrix_t inertia;
  vector_t force;
  vector_t torque;
} body_info_t;

// Create a state vector.
inline state_t *state(vector_t position, vector_t linear_momentum, quaternion_t orientation, vector_t angular_momentum) {
  state_t *result = GC_MALLOC_ATOMIC(sizeof(state_t));
  result->position = position;
  result->linear_momentum = linear_momentum;
  result->orientation = orientation;
  result->angular_momentum = angular_momentum;
  return result;
}

void *runge_kutta(void *y0, double dt, void *f(double, double, void *, void *), void *add(void *, void *),
                  void *scale(void *, double), void *data);

// Determine angular velocity of rotated object with known angular momentum.
inline vector_t angular_velocity(matrix_t inertia, quaternion_t orientation, vector_t angular_momentum) {
  return matrix_vector_dot(inverse(rotate_matrix(orientation, inertia)), angular_momentum);
}

void *state_change(double t, double dt, void *s_, void *data_);
