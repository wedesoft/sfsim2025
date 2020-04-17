#pragma once
#include <tgmath.h>
#include "vector.h"


typedef struct {
  double m11; double m12; double m13;
  double m21; double m22; double m23;
  double m31; double m32; double m33;
} matrix_t;

// Initialize a matrix.
static inline matrix_t matrix(double m11, double m12, double m13,
                              double m21, double m22, double m23,
                              double m31, double m32, double m33) {
  return (matrix_t){.m11 = m11, .m12 = m12, .m13 = m13, .m21 = m21, .m22 = m22, .m23 = m23, .m31 = m31, .m32 = m32, .m33 = m33};
}

// The skew-symmetric cross-product matrix a~ so that a~ * b = a x b.
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

// Initialize a diagonal matrix.
static inline matrix_t diagonal(double m11, double m22, double m33) {
  return matrix(m11, 0, 0, 0, m22, 0, 0, 0, m33);
}

// Create an identity matrix.
static inline matrix_t identity(void) {
  return matrix(1, 0, 0, 0, 1, 0, 0, 0, 1);
}

// Scale matrix elements with a factor.
static inline matrix_t matrix_scale(matrix_t m, double s) {
  return matrix(m.m11 * s, m.m12 * s, m.m13 * s, m.m21 * s, m.m22 * s, m.m23 * s, m.m31 * s, m.m32 * s, m.m33 * s);
}

// Negate matrix.
static inline matrix_t matrix_negative(matrix_t m) {
  return matrix(-m.m11, -m.m12, -m.m13, -m.m21, -m.m22, -m.m23, -m.m31, -m.m32, -m.m33);
}

static inline matrix_t rotation_x(double angle) {
  double ca = cos(angle);
  double sa = sin(angle);
  return matrix(1, 0, 0, 0, ca, -sa, 0, sa, ca);
}

static inline matrix_t rotation_y(double angle) {
  double ca = cos(angle);
  double sa = sin(angle);
  return matrix(ca, 0, sa, 0, 1, 0, -sa, 0, ca);
}

static inline matrix_t rotation_z(double angle) {
  double ca = cos(angle);
  double sa = sin(angle);
  return matrix(ca, -sa, 0, sa, ca, 0, 0, 0, 1);
}
