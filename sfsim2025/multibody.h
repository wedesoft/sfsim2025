#pragma once
#include "large_matrix.h"
#include "list.h"
#include "rigid_body.h"
#include "quaternion.h"


large_matrix_t state_adapter(int n, quaternion_t q[]);

large_matrix_t generalized_mass(int n, double mass[], matrix_t inertia[]);

large_matrix_t contact_normals(int k, vector_t normals[]);

large_matrix_t contact_conditions(list_t *contacts, list_t *bodies);
