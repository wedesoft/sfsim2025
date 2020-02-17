#include <gc.h>
#include <string.h>
#include <tgmath.h>
#include "projection.h"


float *projection(int width, int height, float near, float far, float field_of_view)
{
  float *result = GC_MALLOC_ATOMIC(16 * sizeof(float));
  float d = 1 / tan(field_of_view * M_PI / 360);
  float d2 = d * width / height;
  memset(result, 0, 16 * sizeof(double));
  result[ 0] = d;
  result[ 5] = d2;
  result[10] = (near + far) / (near - far);
  result[11] = -1.0f;
  result[14] = 2 * near * far / (near - far);
  return result;
}

