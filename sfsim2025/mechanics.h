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

// Predict speed and rotation for next time step using specified forces and impulses.
static inline state_t *predict(state_t *s, body_t b, vector_t force, vector_t torque,
                               vector_t linear_impulse, vector_t angular_impulse, double dt) {
  vector_t speed = vector_add(s->speed, speed_change(b, force, linear_impulse, dt));
  vector_t rotation = vector_add(s->rotation, rotation_change(s, b, torque, angular_impulse, dt));
  return state(s->position, speed, s->orientation, rotation);
}
