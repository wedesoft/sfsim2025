#include "linear_complementary.h"
#include "multibody.h"

void frictionless_case(list_t body_infos, list_t states, list_t contacts, large_matrix_t *a, large_vector_t *b, double dt) {
  large_matrix_t n = contact_normals(contacts);
  large_matrix_t j = contact_conditions(contacts, states);
  large_matrix_t m = generalized_mass(body_infos, states);
  large_vector_t u = velocity_vector(states);
  large_vector_t f_ext = external_forces(states, body_infos);
  large_matrix_t n_t = large_transpose(n);
  large_matrix_t j_t = large_transpose(j);
  large_matrix_t m_i = large_inverse(m);
  *a = large_matrix_dot(large_matrix_dot(large_matrix_dot(large_matrix_dot(n_t, j_t), m_i), j), n);
  *b = large_matrix_vector_dot(large_matrix_dot(n_t, j_t),
                               large_vector_add(u, large_vector_scale(large_matrix_vector_dot(m_i, f_ext), dt)));
}
