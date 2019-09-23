#pragma once
#include <math.h>


typedef struct {
  double x;
  double y;
  double z;
} vector_t;

inline vector_t vector(double x, double y, double z) {
  vector_t result;
  result.x = x;
  result.y = y;
  result.z = z;
  return result;
}

inline vector_t negative(vector_t v)
{
  return vector(-v.x, -v.y, -v.z);
}

inline vector_t difference(vector_t a, vector_t b) {
  vector_t result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
  result.z = a.z - b.z;
  return result;
}

inline vector_t cross_product(vector_t a, vector_t b) {
  vector_t result;
  result.x = a.y * b.z - a.z * b.y;
  result.y = a.z * b.x - a.x * b.z;
  result.z = a.x * b.y - a.y * b.x;
  return result;
}

// Dot product of two vectors.
inline double inner_product(vector_t a, vector_t b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Return length of vector.
inline double norm(vector_t v) {
  return sqrt(inner_product(v, v));
}

// Return normalized vector of length one.
inline vector_t normalize(vector_t v) {
  double n = norm(v);
  return vector(v.x / n, v.y / n, v.z / n);
}

vector_t not_parallel(vector_t v);

// Get a vector which is orthogonal to the given vector.
inline vector_t orthogonal1(vector_t v) { return normalize(cross_product(not_parallel(v), v)); }

// Get a second vector which is orthogonal to the given vector.
inline vector_t orthogonal2(vector_t v) { return normalize(cross_product(v, orthogonal1(v))); }
