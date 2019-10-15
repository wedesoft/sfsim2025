#include "multibody.h"
#include "matrix43.h"


// Create adapter matrix for position and orientation changes of multiple bodies.
// http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.39.7671
large_matrix_t state_adapter(int n, quaternion_t q[]) {
  int rows = 7 * n;
  int columns = 6 * n;
  large_matrix_t result = allocate_large_matrix(rows, columns);
  memset(result.data, 0, rows * columns * sizeof(double));
  double *p = result.data;
  for (int l=0; l<n; l++) {
    p[0] = 1; p += columns;
    p[1] = 1; p += columns;
    p[2] = 1; p += columns;
    p += 3;
    matrix43_t o = quaternion_matrix(q[l]);
    p[0] =  o.m11; p[1] = o.m12; p[2] = o.m13; p += columns;
    p[0] =  o.m21; p[1] = o.m22; p[2] = o.m23; p += columns;
    p[0] =  o.m31; p[1] = o.m32; p[2] = o.m33; p += columns;
    p[0] =  o.m41; p[1] = o.m42; p[2] = o.m43; p += columns;
    p += 3;
  };
  return result;
}
