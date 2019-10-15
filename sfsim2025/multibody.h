#pragma once
#include "large_matrix.h"
#include "quaternion.h"


large_matrix_t state_adapter(int n, quaternion_t q[]);

large_matrix_t generalized_mass(int n, double mass[], matrix_t inertia[]);
