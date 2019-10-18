#pragma once
#include "matrix.h"
#include "vector.h"


typedef struct {
  double mass;
  matrix_t inertia;
  vector_t force;
  vector_t torque;
} body_info_t;

static inline body_info_t body_info(double mass, matrix_t inertia, vector_t force, vector_t torque) {
  return (body_info_t){.mass = mass, .inertia = inertia, .force = force, .torque = torque};
}
