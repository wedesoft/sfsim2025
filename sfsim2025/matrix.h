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

// Dot product of two matrices.
inline matrix_t matrix_dot(matrix_t a, matrix_t b) {
  return matrix(a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31,
                a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32,
                a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33,
                a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31,
                a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32,
                a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33,
                a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31,
                a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32,
                a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33);
}

// Compute inverse matrix.
inline matrix_t inverse(matrix_t m)
{
  // https://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/threeD/index.htm
  matrix_t result;
  double det = m.m11 * m.m22 * m.m33 + m.m12 * m.m23 * m.m31 + m.m13 * m.m21 * m.m32
             - m.m11 * m.m23 * m.m32 - m.m12 * m.m21 * m.m33 - m.m13 * m.m22 * m.m31;
  result.m11 = (m.m22 * m.m33 - m.m23 * m.m32) / det;
  result.m12 = (m.m13 * m.m32 - m.m12 * m.m33) / det;
  result.m13 = (m.m12 * m.m23 - m.m13 * m.m22) / det;
  result.m21 = (m.m23 * m.m31 - m.m21 * m.m33) / det;
  result.m22 = (m.m11 * m.m33 - m.m13 * m.m31) / det;
  result.m23 = (m.m13 * m.m21 - m.m11 * m.m23) / det;
  result.m31 = (m.m21 * m.m32 - m.m22 * m.m31) / det;
  result.m32 = (m.m12 * m.m31 - m.m11 * m.m32) / det;
  result.m33 = (m.m11 * m.m22 - m.m12 * m.m21) / det;
  return result;
}
