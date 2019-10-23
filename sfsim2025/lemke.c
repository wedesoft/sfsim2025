#include "lemke.h"

large_matrix_t initialize_tableau(large_matrix_t a, large_vector_t b) {
  int columns = 2 * a.cols + 2;
  large_matrix_t result = allocate_large_matrix(a.rows, columns);
  memset(result.data, 0, a.rows * columns * sizeof(double));
  double *p = result.data;
  double *d = a.data;
  double *q = b.data;
  for (int j=0; j<a.rows; j++) {
    p[j] = 1.0;
    p += a.cols;
    for (int i=0; i<a.cols; i++) {
      *p++ = -*d++;
    };
    *p++ = -1.0;
    *p++ = *q++;
  };
  return result;
}
