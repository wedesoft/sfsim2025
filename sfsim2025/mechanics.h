#pragma once
#include "vector.h"
#include "quaternion.h"


typedef struct {
  vector_t position;
  vector_t linear_momentum;
  quaternion_t orientation;
  vector_t angular_momentum;
} state_t;

inline state_t state(vector_t position, vector_t linear_momentum, quaternion_t orientation, vector_t angular_momentum) {
  state_t result = {
    .position = position,
    .linear_momentum = linear_momentum,
    .orientation = orientation,
    .angular_momentum = angular_momentum
  };
  return result;
}

void *runge_kutta(double t0, void *y0, double dt, void *f(double, void *), void *add(void *, void *),
                  void *scale(void *, double));
