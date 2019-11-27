#pragma once
#include <gc.h>
#include "body.h"
#include "forces.h"
#include "state.h"


void *runge_kutta(void *y0, double dt, void *f(double, double, void *, void *), void *add(void *, void *),
                  void *scale(void *, double), void *data);

// Euler integration step.
static inline void *euler(void *y0, double dt, void *f(double, double, void *, void *), void *add(void *, void *),
                          void *scale(void *, double), void *data) {
  return add(y0, f(0, dt, y0, data));
}

typedef struct {
  body_t body;
  forces_t forces;
} body_info_t;

static inline body_info_t body_info(body_t body, forces_t forces) {
  return (body_info_t){.body = body, .forces = forces};
}

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

state_t *predict(state_t *s, body_t b, forces_t f, vector_t p, vector_t t, double dt);
