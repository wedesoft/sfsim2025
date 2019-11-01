#pragma once
#include "body_info.h"
#include "large_matrix.h"
#include "large_vector.h"
#include "state.h"
#include "vector.h"


typedef struct {
  vector_t r1;
  vector_t r2;
} joint_t;

static inline joint_t joint(vector_t r1, vector_t r2) {
  return (joint_t){.r1 = r1, .r2 = r2};
}

large_matrix_t joint_mass(body_info_t info1, body_info_t info2, state_t *state1, state_t *state2);

large_vector_t speed_vector(state_t *state1, state_t *state2);

large_matrix_t ball_in_socket_jacobian(state_t *state1, state_t *state2, joint_t joint);

large_vector_t ball_in_socket_correction(state_t *state1, state_t *state2, joint_t joint);

void correcting_impulse(body_info_t info1, body_info_t info2, state_t *state1, state_t *state2, large_matrix_t j, large_vector_t b,
                        vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2);
