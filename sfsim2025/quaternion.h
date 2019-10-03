#pragma once
#include <stdbool.h>
#include <tgmath.h>
#include "vector.h"
#include "matrix.h"


typedef struct {
  double a;
  double b;
  double c;
  double d;
} quaternion_t;

// Initialize quaternion.
inline quaternion_t quaternion(double a, double b, double c, double d) {
  return (quaternion_t){.a = a, .b = b, .c = c, .d = d};
}

// Equality of quaternions.
inline bool quaternion_eq(quaternion_t q, quaternion_t r) {
  return q.a == r.a && q.b == r.b && q.c == r.c && q.d == r.d;
}

// Create quaternion real component zero and other components taken from the vector.
inline quaternion_t vector_to_quaternion(vector_t v) {
  return quaternion(0, v.x, v.y, v.z);
}

// Create vector from imaginary, jmaginary, and kmaginary component of quaternion.
inline vector_t quaternion_to_vector(quaternion_t q) {
  return vector(q.b, q.c, q.d);
}

// Sinus x divided by x.
inline double sinc(double x) {
  return x == 0 ? 1.0 : sin(x) / x;
}

quaternion_t quaternion_exp(quaternion_t q);

// Determine rotation quaternion from angle value and axis vector.
inline quaternion_t quaternion_rotation(double theta, vector_t axis) {
  return quaternion_exp(vector_to_quaternion(vector_scale(axis, 0.5 * theta)));
}

// Get conjugate of a quaternion.
inline quaternion_t quaternion_conjugate(quaternion_t q) {
  double scale = 1 / (q.a * q.a + q.b * q.b + q.c * q.c + q.d * q.d);
  return quaternion(q.a * scale, -q.b * scale, -q.c * scale, -q.d * scale);
}

// Add two quaternions.
inline quaternion_t quaternion_add(quaternion_t q, quaternion_t r) {
  return quaternion(q.a + r.a, q.b + r.b, q.c + r.c, q.d + r.d);
}

// Get product of two quaternions.
inline quaternion_t quaternion_product(quaternion_t q, quaternion_t r) {
  return quaternion(q.a * r.a - q.b * r.b - q.c * r.c - q.d * r.d,
                    q.a * r.b + q.b * r.a + q.c * r.d - q.d * r.c,
                    q.a * r.c - q.b * r.d + q.c * r.a + q.d * r.b,
                    q.a * r.d + q.b * r.c - q.c * r.b + q.d * r.a);
}

// Use quaternion to rotate a vector.
inline vector_t rotate_vector(quaternion_t q, vector_t v) {
  return quaternion_to_vector(quaternion_product(quaternion_product(q, vector_to_quaternion(v)), quaternion_conjugate(q)));
}

matrix_t rotation_matrix(quaternion_t q);

matrix_t rotate_matrix(quaternion_t q, matrix_t m);
