#pragma once
#include <string.h>
#include <gc.h>


typedef struct {
  int rows;
  double *data;
} large_vector_t;

// Allocate large vector.
static inline large_vector_t allocate_large_vector(int rows) {
  return (large_vector_t){.rows = rows, .data = GC_MALLOC_ATOMIC(rows * sizeof(double))};
}

// Allocate and initialize large vector.
static inline large_vector_t large_vector(int rows, double *data) {
  large_vector_t result = allocate_large_vector(rows);
  memcpy(result.data, data, rows * sizeof(double));
  return result;
}
