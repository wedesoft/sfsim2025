#include <gc.h>
#include <stdlib.h>
#include <stdio.h>
#include "sfsim2025/joint.h"


int main(int argc, char *argv[]) {
  GC_INIT();
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(4, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  body_info_t info1 = body_info(1.0, diagonal(1, 1, 1), vector(0, 0, 0), vector(0, 0, 0));
  body_info_t info2 = body_info(1.0, diagonal(1, 1, 1), vector(0, 0, 0), vector(0, 0, 0));
  large_matrix_t j = ball_in_socket(s1, s2, vector(2, 0, 0), vector(-2, 0, 0));
  large_matrix_t m = joint_mass(info1, info2, s1, s2);
  large_vector_t u = speed_vector(s1, s2);
  large_matrix_t d = large_inverse(large_matrix_dot(large_matrix_dot(j, large_inverse(m)), large_transpose(j)));
  large_vector_t v = large_matrix_vector_dot(j, u);
  large_vector_t l = large_vector_scale(large_matrix_vector_dot(d, v), -1);
  large_vector_t p = large_matrix_vector_dot(large_transpose(j), l);
  for (int i=0; i<p.rows; i++)
    printf("%f\n", p.data[i]);
  return EXIT_SUCCESS;
}
