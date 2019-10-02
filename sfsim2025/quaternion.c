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

