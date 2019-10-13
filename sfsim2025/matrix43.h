#pragma once
#include "quaternion.h"


typedef struct {
  double m11; double m12; double m13;
  double m21; double m22; double m23;
  double m31; double m32; double m33;
  double m41; double m42; double m43;
} matrix43_t;

// Initialize a 4x3 matrix.
static inline matrix43_t matrix43(double m11, double m12, double m13,
                                  double m21, double m22, double m23,
                                  double m31, double m32, double m33,
                                  double m41, double m42, double m43) {
  return (matrix43_t){.m11 = m11, .m12 = m12, .m13 = m13,
                      .m21 = m21, .m22 = m22, .m23 = m23,
                      .m31 = m31, .m32 = m32, .m33 = m33,
                      .m41 = m41, .m42 = m42, .m43 = m43};
}

// Multiply 4x3 matrix with vector and return result as a quaternion.
static inline quaternion_t matrix43_vector_dot(matrix43_t m, vector_t v) {
  return quaternion(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z,
                    m.m21 * v.x + m.m22 * v.y + m.m23 * v.z,
                    m.m31 * v.x + m.m32 * v.y + m.m33 * v.z,
                    m.m41 * v.x + m.m42 * v.y + m.m43 * v.z);
}

// Generate matrix for replacing quaternion multiplication and factor 0.5 with matrix-vector dot product.
static inline matrix43_t quaternion_matrix(quaternion_t q) {
  return matrix43(-q.b * 0.5, -q.c * 0.5, -q.d * 0.5,
                   q.a * 0.5,  q.d * 0.5, -q.c * 0.5,
                  -q.d * 0.5,  q.a * 0.5,  q.b * 0.5,
                   q.c * 0.5, -q.b * 0.5,  q.a * 0.5);
}
