#include <clapack.h>
#include "matrix.h"


// Compute inverse matrix.
matrix_t inverse(matrix_t m) {
  // https://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/threeD/index.htm
  // https://stackoverflow.com/questions/3519959/computing-the-inverse-of-a-matrix-using-lapack-in-c
  double a[9] = {m.m11, m.m12, m.m13, m.m21, m.m22, m.m23, m.m31, m.m32, m.m33};
  int ipiv[3];
  clapack_dgetrf(CblasRowMajor, 3, 3, a, 3, ipiv);
  clapack_dgetri(CblasRowMajor, 3, a, 3, ipiv);
  return (matrix_t){
    .m11 = a[0], .m12 = a[1], .m13 = a[2], .m21 = a[3], .m22 = a[4], .m23 = a[5], .m31 = a[6], .m32 = a[7], .m33 = a[8]
  };
}
