#include <stdio.h>
#include "sfsim2025/body_info.h"
#include "sfsim2025/list.h"
#include "sfsim2025/mechanics.h"
#include "sfsim2025/multibody.h"
#include "sfsim2025/rigid_body.h"
#include "sfsim2025/state.h"

large_matrix_t t(large_matrix_t m) {
  return large_transpose(m);
}

large_matrix_t x(large_matrix_t a, large_matrix_t b) {
  return large_matrix_dot(a, b);
}

large_matrix_t inv(large_matrix_t m) {
  return large_inverse(m);
}

large_vector_t xx(large_matrix_t m, large_vector_t v) {
  return large_matrix_vector_dot(m, v);
}

large_vector_t add(large_vector_t a, large_vector_t b) {
  return large_vector_add(a, b);
}

int main(void) {
  GC_INIT();
  state_t *s1 = state(vector(0, 0, 6370e+3 + 0.1), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  list_t states = make_list();
  append_pointer(&states, s1);
  append_pointer(&states, s2);

  body_info_t info1 = body_info(1.0, inertia_cuboid(1.0, 1, 0.4, 0.2), vector(0, 0, -10), vector(0, 0, 0));
  body_info_t info2 = body_info(6e+24, inertia_cuboid(6e+24, 6370e+3, 6370e+3, 6370e+3), vector(0, 0, 0), vector(0, 0, 0));
  list_t body_infos = make_list();
  append_body_info(&body_infos, info1);
  append_body_info(&body_infos, info2);

  vector_t normal = vector(0, 0, -1);
  contact_t c = contact(0, 1, normal, vector(0, 0, 6370e+3));
  list_t contacts = make_list();
  append_contact(&contacts, c);

  large_matrix_t n = contact_normals(1, &normal);
  large_matrix_t j = contact_conditions(contacts, states);
  large_matrix_t m = generalized_mass(body_infos, states);
  large_vector_t u = velocity_vector(states);
  large_vector_t f_ext = external_forces(states, body_infos);
  large_matrix_t a = x(x(x(x(t(n), t(j)), inv(m)), j), n);
  large_vector_t b = xx(x(t(n), t(j)), add(u, xx(inv(m), f_ext)));
  printf("%f * x + %f >= 0; x >= 0\n", a.data[0], b.data[0]);
  return 0;
}