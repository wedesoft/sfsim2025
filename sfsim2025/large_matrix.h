#pragma once
#include <string.h>
#include <gc.h>
#include "large_vector.h"


typedef struct {
  int rows;
  int cols;
  double *data;
} large_matrix_t;

// Allocate large matrix.
static inline large_matrix_t allocate_large_matrix(int rows, int cols) {
  return (large_matrix_t){.rows = rows, .cols = cols, .data = GC_MALLOC_ATOMIC(rows * cols * sizeof(double))};
}

// Allocate and initialize large matrix.
static inline large_matrix_t large_matrix(int rows, int cols, double *data) {
  large_matrix_t result = allocate_large_matrix(rows, cols);
  memcpy(result.data, data, rows * cols * sizeof(double));
  return result;
}

large_matrix_t large_matrix_dot(large_matrix_t a, large_matrix_t b);

large_matrix_t large_transpose(large_matrix_t a);

large_matrix_t large_inverse(large_matrix_t a);

large_vector_t large_matrix_vector_dot(large_matrix_t m, large_vector_t v);
