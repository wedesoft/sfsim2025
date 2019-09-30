#pragma once
#include "vector.h"


typedef struct {
  double m11;
  double m12;
  double m13;
  double m21;
  double m22;
  double m23;
  double m31;
  double m32;
  double m33;
} matrix_t;

inline matrix_t matrix(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33) {
  matrix_t result;
  result.m11 = m11; result.m12 = m12; result.m13 = m13;
  result.m21 = m21; result.m22 = m22; result.m23 = m23;
  result.m31 = m31; result.m32 = m32; result.m33 = m33;
  return result;
}

// Cross-product matrix a~ so that a~*b is a x b.
inline matrix_t cross_product_matrix(vector_t v) {
  return matrix(0, -v.z, v.y, v.z, 0, -v.x, -v.y, v.x, 0);
}

// Dot product of matrix and vector.
inline vector_t matrix_vector_dot(matrix_t m, vector_t v) {
  return vector(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z,
                m.m21 * v.x + m.m22 * v.y + m.m23 * v.z,
                m.m31 * v.x + m.m32 * v.y + m.m33 * v.z);
}

// Tramspose a matrix.
inline matrix_t transpose(matrix_t m) {
  return matrix(m.m11, m.m21, m.m31, m.m12, m.m22, m.m32, m.m13, m.m23, m.m33);
};
