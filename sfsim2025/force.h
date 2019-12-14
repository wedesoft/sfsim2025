#pragma once
#include "body.h"
#include "state.h"


typedef enum {GRAVITATION} force_type_t;

typedef struct {
  force_type_t force_type;
  int i;
  int j;
} force_t;

static inline force_t gravitation(int i, int j) {
  return (force_t){.force_type = GRAVITATION, .i = i, .j = j};
}

void exert_force(body_t body1, body_t body2, force_t force, state_t *state1, state_t *state2,
                 vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2);
