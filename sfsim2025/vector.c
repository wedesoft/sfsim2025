#include "vector.h"

vector_t not_parallel(vector_t v) {
  if (fabs(v.x) <= fabs(v.y))
    if (fabs(v.x) <= fabs(v.z))
      return vector(1, 0, 0);
    else
      return vector(0, 0, 1);
  else
    if (fabs(v.y) < fabs(v.z))
      return vector(0, 1, 0);
    else
      return vector(0, 0, 1);
}
