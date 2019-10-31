#include <assert.h>
#include "large_vector.h"


// Add two large vectors.
large_vector_t large_vector_add(large_vector_t a, large_vector_t b) {
  assert(a.rows == b.rows);
  large_vector_t result = allocate_large_vector(a.rows);
  double *p = result.data;
  double *q = a.data;
  double *r = b.data;
  for (int i=0; i<result.rows; i++)
    *p++ = *q++ + *r++;
  return result;
}

// Multiply vector with a scalar factor.
large_vector_t large_vector_scale(large_vector_t a, double s) {
  large_vector_t result = allocate_large_vector(a.rows);
  double *p = result.data;
  double *q = a.data;
  for (int i=0; i<result.rows; i++)
    *p++ = *q++ * s;
  return result;
}

// Element-wise negative of vector.
large_vector_t large_vector_negative(large_vector_t a) {
  large_vector_t result = allocate_large_vector(a.rows);
  double *p = result.data;
  double *q = a.data;
  for (int i=0; i<result.rows; i++)
    *p++ = -*q++;
  return result;
}
