#pragma once
#include <string.h>
#include <gc.h>
#include "vector.h"


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

large_vector_t large_vector_add(large_vector_t a, large_vector_t b);

large_vector_t large_vector_scale(large_vector_t a, double s);

large_vector_t large_vector_negative(large_vector_t a);

static inline large_vector_t to_large_vector(vector_t v) {
  large_vector_t result = allocate_large_vector(3);
  result.data[0] = v.x;
  result.data[1] = v.y;
  result.data[2] = v.z;
  return result;
}
