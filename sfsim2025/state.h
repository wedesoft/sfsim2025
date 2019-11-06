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

void *scale_state(void *s_, double scale);
