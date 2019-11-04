#pragma once
#include "vector.h"


typedef struct {
  vector_t force;
  vector_t torque;
} forces_t;

static inline forces_t forces(vector_t force, vector_t torque) {
  return (forces_t){.force = force, .torque = torque};
}
