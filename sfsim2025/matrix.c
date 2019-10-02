#include "matrix.h"


// Compute inverse matrix.
matrix_t inverse(matrix_t m) {
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
