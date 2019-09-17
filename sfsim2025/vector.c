#include <math.h>
#include "vector.h"


vector_t normalize(vector_t v)
{
  float norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  return vector(v.x / norm, v.y / norm, v.z / norm);
}

