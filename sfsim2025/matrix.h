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

static inline matrix_t matrix(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33) {
  return (matrix_t){.m11 = m11, .m12 = m12, .m13 = m13, .m21 = m21, .m22 = m22, .m23 = m23, .m31 = m31, .m32 = m32, .m33 = m33};
}

// Skew-symmetric cross-product matrix a~ so that a~*b is a x b.
static inline matrix_t cross_product_matrix(vector_t v) {
  return matrix(0, -v.z, v.y, v.z, 0, -v.x, -v.y, v.x, 0);
}

// Dot product of matrix and vector.
static inline vector_t matrix_vector_dot(matrix_t m, vector_t v) {
  return vector(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z,
                m.m21 * v.x + m.m22 * v.y + m.m23 * v.z,
                m.m31 * v.x + m.m32 * v.y + m.m33 * v.z);
}

// Tramspose a matrix.
static inline matrix_t transpose(matrix_t m) {
  return matrix(m.m11, m.m21, m.m31, m.m12, m.m22, m.m32, m.m13, m.m23, m.m33);
};

// Dot product of two matrices.
static inline matrix_t matrix_dot(matrix_t a, matrix_t b) {
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

matrix_t inverse(matrix_t m);

static inline matrix_t diagonal(double m11, double m22, double m33) {
  return matrix(m11, 0, 0, 0, m22, 0, 0, 0, m33);
}
