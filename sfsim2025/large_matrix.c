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
        p++;
        q += b.cols;
      };
      *r++ = cell;
    };
    p0 += a.cols;
  };
  return result;
}

// Return transposed matrix.
large_matrix_t large_transpose(large_matrix_t a) {
  large_matrix_t result = allocate_large_matrix(a.cols, a.rows);
  double *r = result.data;
  double *p0 = a.data;
  for (int j=0; j<a.cols; j++) {
    double *p = p0;
    for (int i=0; i<a.rows; i++) {
      *r = *p;
      p += a.cols;
      r++;
    };
    p0++;
  };
  return result;
}

// Compute inverse of large matrix.
large_matrix_t large_inverse(large_matrix_t a) {
  // https://stackoverflow.com/questions/3519959/computing-the-inverse-of-a-matrix-using-lapack-in-c
  assert(a.rows == a.cols);
  large_matrix_t result = allocate_large_matrix(a.rows, a.cols);
  memcpy(result.data, a.data, a.rows * a.cols * sizeof(double));
  int ipiv[a.rows];
  clapack_dgetrf(CblasRowMajor, a.rows, a.cols, result.data, a.rows, ipiv);
  clapack_dgetri(CblasRowMajor, a.rows, result.data, a.rows, ipiv);
  return result;
}

large_vector_t large_matrix_vector_dot(large_matrix_t m, large_vector_t v) {
  assert(m.cols == v.rows);
  large_vector_t result = allocate_large_vector(m.rows);
  double *r = result.data;
  double *p0 = m.data;
  for (int j=0; j<m.rows; j++) {
    double cell = 0;
    double *p = p0;
    double *q = v.data;
    for (int i=0; i<m.cols; i++) {
      cell += *p * * q;
      p++;
      q++;
    };
    *r++ = cell;
    p0 += m.cols;
  };
  return result;
}

large_matrix_t projection(int width, int height, float near, float far, float field_of_view)
{
  large_matrix_t result = allocate_large_matrix(4, 4);
  float d = 1 / tan(field_of_view * M_PI / 360);
  float d2 = d * width / height;
  memset(result.data, 0, 4 * 4 * sizeof(float));
  result.data[ 0] = d;
  result.data[ 5] = d2;
  result.data[10] = (near + far) / (near - far);
  result.data[14] = -1.0f;
  result.data[11] = 2 * near * far / (near - far);
  return result;
}
