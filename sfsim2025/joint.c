#include "joint.h"


// Create mass and inertia matrix for a pair of bodies.
large_matrix_t joint_mass(body_info_t info1, body_info_t info2, state_t *state1, state_t *state2) {
  large_matrix_t result = allocate_large_matrix(12, 12);
  memset(result.data, 0, result.rows * result.cols * sizeof(double));
  double *p = result.data;
  *p = info1.mass; p += 13; *p = info1.mass; p += 13; *p = info1.mass; p += 13;
  matrix_t inertia1 = rotate_matrix(state1->orientation, info1.inertia);
  *p++ = inertia1.m11; *p++ = inertia1.m12; *p++ = inertia1.m13; p += 9;
  *p++ = inertia1.m21; *p++ = inertia1.m22; *p++ = inertia1.m23; p += 9;
  *p++ = inertia1.m31; *p++ = inertia1.m32; *p++ = inertia1.m33; p += 12;
  *p = info2.mass; p += 13; *p = info2.mass; p += 13; *p = info2.mass; p += 13;
  matrix_t inertia2 = rotate_matrix(state2->orientation, info2.inertia);
  *p++ = inertia2.m11; *p++ = inertia2.m12; *p++ = inertia2.m13; p += 9;
  *p++ = inertia2.m21; *p++ = inertia2.m22; *p++ = inertia2.m23; p += 9;
  *p++ = inertia2.m31; *p++ = inertia2.m32; *p++ = inertia2.m33; p += 12;
  return result;
}

large_vector_t speed_vector(state_t *state1, state_t *state2) {
  large_vector_t result = allocate_large_vector(12);
  result.data[ 0] = state1->speed.x;
  result.data[ 1] = state1->speed.y;
  result.data[ 2] = state1->speed.z;
  result.data[ 3] = state1->rotation.x;
  result.data[ 4] = state1->rotation.y;
  result.data[ 5] = state1->rotation.z;
  result.data[ 6] = state2->speed.x;
  result.data[ 7] = state2->speed.y;
  result.data[ 8] = state2->speed.z;
  result.data[ 9] = state2->rotation.x;
  result.data[10] = state2->rotation.y;
  result.data[11] = state2->rotation.z;
  return result;
}
