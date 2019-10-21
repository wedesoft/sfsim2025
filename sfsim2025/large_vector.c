#include <assert.h>
#include "large_vector.h"


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
