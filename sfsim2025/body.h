#pragma once
#include "matrix.h"


typedef struct {
  double mass;
  matrix_t inertia;
} body_t;

static inline body_t body(double mass, matrix_t inertia) {
  return (body_t){.mass = mass, .inertia = inertia};
}
