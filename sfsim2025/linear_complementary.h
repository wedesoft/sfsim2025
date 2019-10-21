#pragma once
#include "body_info.h"
#include "large_matrix.h"
#include "list.h"
#include "state.h"

void frictionless_case(list_t body_infos, list_t states, list_t contacts, large_matrix_t *a, large_vector_t *b);
