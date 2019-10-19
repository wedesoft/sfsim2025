#include "sfsim2025/body_info.h"
#include "sfsim2025/list.h"
#include "sfsim2025/mechanics.h"
#include "sfsim2025/multibody.h"
#include "sfsim2025/rigid_body.h"
#include "sfsim2025/state.h"

int main(void) {
  GC_INIT();
  body_info_t info1 = body_info(1.0, inertia_cuboid(1.0, 1, 0.4, 0.2), vector(0, 0, -10), vector(0, 0, 0));
  state_t *s1 = state(vector(0, 0, 6370e+3 + 0.1), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  rigid_body_t *r1 = make_rigid_body(vector(0, 0, 6370e+3 + 0.1));
  body_info_t info2 = body_info(6e+24, inertia_cuboid(6e+24, 6370e+3, 6370e+3, 6370e+3), vector(0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  rigid_body_t *r2 = make_rigid_body(vector(0, 0, 0));
  vector_t normal = vector(0, 0, 1);
  contact_t c = contact(0, 1, normal, vector(0, 0, 6370e+3));
  list_t contacts = make_list();
  append_contact(&contacts, c);
  list_t bodies = make_list();
  append_pointer(&bodies, r1);
  append_pointer(&bodies, r2);
  large_matrix_t n = contact_normals(1, &normal);
  large_matrix_t j = contact_conditions(contacts, bodies);
  double mass[] = {1.0, 6e+24};
  matrix_t inertia[] = {info1.inertia, info2.inertia};
  quaternion_t orientation[] = {s1->orientation, s2->orientation};
  large_matrix_t m = generalized_mass(2, mass, inertia, orientation);
  list_t states = make_list();
  append_pointer(&states, s1);
  append_pointer(&states, s2);
  large_vector_t u = velocity_vector(states);
  list_t body_infos = make_list();
  append_body_info(&body_infos, info1);
  append_body_info(&body_infos, info2);
  large_vector_t f_ext = external_forces(states, body_infos);
  return 0;
}
