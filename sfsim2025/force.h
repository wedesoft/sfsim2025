#pragma once
#include "body.h"
#include "state.h"


typedef enum {GRAVITATION, FIXED_FORCE, SPRING_DAMPER} force_type_t;

typedef struct {
  vector_t force;
  vector_t torque;
} fixed_force_t;

typedef struct {
  vector_t r1;
  vector_t r2;
  double length;
  double k;
  double c;
} spring_damper_t;

typedef struct {
  force_type_t force_type;
  int i;
  int j;
  union {
    fixed_force_t fixed_force;
    spring_damper_t spring_damper;
  };
} force_t;

static inline force_t gravitation(int i, int j) {
  return (force_t){.force_type = GRAVITATION, .i = i, .j = j};
}

static inline force_t fixed_force(int i, vector_t force, vector_t torque) {
  return (force_t){.force_type = FIXED_FORCE, .i = i, .j = i, .fixed_force.force = force, .fixed_force.torque = torque};
}

static inline force_t spring_damper(int i, int j, vector_t r1, vector_t r2, double length, double k, double c) {
  return (force_t){.force_type = SPRING_DAMPER, .i = i, .j = j, .spring_damper.r1 = r1, .spring_damper.r2 = r2,
                   .spring_damper.length = length, .spring_damper.k = k, .spring_damper.c = c};
}

void exert_force(body_t body1, body_t body2, force_t force, state_t *state1, state_t *state2,
                 vector_t *force1, vector_t *force2, vector_t *tau1, vector_t *tau2);
