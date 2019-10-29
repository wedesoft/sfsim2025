#pragma once
#include <gc.h>
#include "body_info.h"
#include "state.h"


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

// Compute inertial matrix of solid sphere.
static inline matrix_t inertia_sphere(double mass, double radius) {
  double i = 2.0 / 5.0 * mass * radius * radius;
  return diagonal(i, i, i);
}
