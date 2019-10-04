#pragma once
#include <stdbool.h>
#include <tgmath.h>


typedef struct {
  double x;
  double y;
  double z;
} vector_t;

// Initialize vector.
static inline vector_t vector(double x, double y, double z) {
  return (vector_t){.x = x, .y = y, .z = z};
}

static inline bool vector_eq(vector_t a, vector_t b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

// Negative vector.
static inline vector_t vector_negative(vector_t v)
{
  return vector(-v.x, -v.y, -v.z);
}

// Add two vectors.
static inline vector_t vector_add(vector_t a, vector_t b) {
  return vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

// Difference of two vectors.
static inline vector_t vector_subtract(vector_t a, vector_t b) {
  return vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

// Multiply a vector with a scalar.
static inline vector_t vector_scale(vector_t a, double s) {
  return vector(a.x * s, a.y * s, a.z * s);
}

// Cross-product of two vectors.
static inline vector_t cross_product(vector_t a, vector_t b) {
  return vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

// Dot product of two vectors.
static inline double inner_product(vector_t a, vector_t b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Return length of vector.
static inline double norm(vector_t v) {
  return sqrt(inner_product(v, v));
}

// Return normalized vector of length one.
static inline vector_t normalize(vector_t v) {
  double n = norm(v);
  return vector(v.x / n, v.y / n, v.z / n);
}

vector_t not_parallel(vector_t v);

// Get a vector which is orthogonal to the given vector.
static inline vector_t orthogonal1(vector_t v) {
  return normalize(cross_product(not_parallel(v), v));
}

// Get a second vector which is orthogonal to the given vector.
static inline vector_t orthogonal2(vector_t v) {
  return normalize(cross_product(v, orthogonal1(v)));
}
