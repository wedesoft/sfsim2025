#pragma once
#include "body_info.h"
#include "large_matrix.h"
#include "large_vector.h"
#include "state.h"


large_matrix_t joint_mass(body_info_t info1, body_info_t info2, state_t *state1, state_t *state2);

large_vector_t speed_vector(state_t *state1, state_t *state2);

large_matrix_t ball_in_socket(state_t *state1, state_t *state2, vector_t r1, vector_t r2);
