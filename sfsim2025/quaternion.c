#include "quaternion.h"


// Exponential of quaternion.
quaternion_t quaternion_exp(quaternion_t q)
{
  double scale = exp(q.a);
  vector_t axis = quaternion_to_vector(q);
  double rotation = norm(axis);
  double cos_rotation = cos(rotation);
  double sinc_rotation = sinc(rotation);
  return quaternion(scale * cos_rotation,
                    scale * sinc_rotation * axis.x,
                    scale * sinc_rotation * axis.y,
                    scale * sinc_rotation * axis.z);
}

matrix_t rotation_matrix(quaternion_t q) {
  vector_t u = rotate_vector(q, vector(1, 0, 0));
  vector_t v = rotate_vector(q, vector(0, 1, 0));
  vector_t w = rotate_vector(q, vector(0, 0, 1));
  return matrix(u.x, v.x, w.x, u.y, v.y, w.y, u.z, v.z, w.z);
}
