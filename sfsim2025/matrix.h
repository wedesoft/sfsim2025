#pragma once
#include <gc.h>
#include "vector.h"


typedef double matrix_t[3][3];

inline matrix_t *matrix(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33) {
  matrix_t *result = GC_MALLOC_ATOMIC(sizeof(matrix_t));
  *result[0][0] = a11; *result[0][1] = a12; *result[0][2] = a13;
  *result[1][0] = a21; *result[1][1] = a22; *result[1][2] = a23;
  *result[2][0] = a31; *result[2][1] = a32; *result[2][2] = a33;
  return result;
}

// Cross-product matrix a~ so that a~*b is a x b.
inline matrix_t *cross_product_matrix(vector_t v) {
  return matrix(0, -v.z, v.y, v.z, 0, -v.x, -v.y, v.x, 0);
}
