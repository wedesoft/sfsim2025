#include <assert.h>
#include <clapack.h>
#include "large_matrix.h"


// Matrix dot product of two large matrices.
large_matrix_t large_matrix_dot(large_matrix_t a, large_matrix_t b) {
  assert(a.cols == b.rows);
  large_matrix_t result = allocate_large_matrix(a.rows, b.cols);
  double *p0 = a.data;
  double *r = result.data;
  for (int j=0; j<a.rows; j++) {
    for (int i=0; i<b.cols; i++) {
      double cell = 0;
      double *p = p0;
      double *q = b.data + i;
      for (int k=0; k<a.cols; k++) {
        cell += *p * *q;
        p += 1;
        q += b.cols;
      };
      *r++ = cell;
    };
    p0 += a.cols;
  };
  return result;
}

// Compute inverse of large matrix.
large_matrix_t large_inverse(large_matrix_t a) {
  assert(a.rows == a.cols);
  large_matrix_t result = allocate_large_matrix(a.rows, a.cols);
  memcpy(result.data, a.data, a.rows * a.cols * sizeof(double));
  int ipiv[a.rows];
  clapack_dgetrf(CblasRowMajor, a.rows, a.cols, result.data, a.rows, ipiv);
  clapack_dgetri(CblasRowMajor, a.rows, result.data, a.rows, ipiv);
  return result;
}
