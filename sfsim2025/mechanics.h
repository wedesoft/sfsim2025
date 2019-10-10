#pragma once
#include <gc.h>
#include "vector.h"
#include "quaternion.h"


typedef struct {
  vector_t position;
  vector_t speed;
  quaternion_t orientation;
  vector_t rotation;
} state_t;

typedef struct {
  double mass;
  matrix_t inertia;
  vector_t force;
  vector_t torque;
} body_info_t;

// Create a state vector.
static inline state_t *state(vector_t position, vector_t speed, quaternion_t orientation, vector_t rotation) {
  state_t *result = GC_MALLOC_ATOMIC(sizeof(state_t));
  result->position = position;
  result->speed = speed;
  result->orientation = orientation;
  result->rotation = rotation;
  return result;
}

void *runge_kutta(void *y0, double dt, void *f(double, double, void *, void *), void *add(void *, void *),
                  void *scale(void *, double), void *data);

void *state_change(double t, double dt, void *s_, void *data_);

void *add_states(void *a_, void *b_);

void *scale_state(void *s_, double scale);

// Compute inertial matrix of cuboid.
static inline matrix_t inertia_cuboid(double mass, double width, double height, double depth) {
  double width2 = width * width;
  double height2 = height * height;
  double depth2 = depth * depth;
  return matrix_scale(diagonal(height2 + depth2, width2 + depth2, width2 + height2), mass / 12);
}
