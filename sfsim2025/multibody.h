#pragma once
#include "large_matrix.h"
#include "large_vector.h"
#include "list.h"
#include "rigid_body.h"
#include "quaternion.h"
#include "state.h"


large_matrix_t state_adapter(int n, quaternion_t q[]);

large_matrix_t generalized_mass(int n, double mass[], matrix_t inertia[]);

large_matrix_t contact_normals(int k, vector_t normals[]);

large_matrix_t contact_conditions(list_t contacts, list_t bodies);

large_vector_t velocity_vector(list_t states);

large_vector_t external_forces(list_t states, list_t body_infos);
