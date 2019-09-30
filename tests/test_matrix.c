#include "sfsim2025/matrix.h"
#include "test_matrix.h"


static MunitResult test_create(const MunitParameter params[], void *data) {
  matrix_t result = matrix(1, 2, 3, 4, 5, 6, 7, 8, 9);
  munit_assert_double(result.m11, ==, 1); munit_assert_double(result.m12, ==, 2); munit_assert_double(result.m13, ==, 3);
  munit_assert_double(result.m21, ==, 4); munit_assert_double(result.m22, ==, 5); munit_assert_double(result.m23, ==, 6);
  munit_assert_double(result.m31, ==, 7); munit_assert_double(result.m32, ==, 8); munit_assert_double(result.m33, ==, 9);
  return MUNIT_OK;
}

static MunitResult test_cross_product_matrix(const MunitParameter params[], void *data) {
  matrix_t result = cross_product_matrix(vector(1, 2, 3));
  munit_assert_double(result.m11, ==,  0); munit_assert_double(result.m12, ==, -3); munit_assert_double(result.m13, ==,  2);
  munit_assert_double(result.m21, ==,  3); munit_assert_double(result.m22, ==,  0); munit_assert_double(result.m23, ==, -1);
  munit_assert_double(result.m31, ==, -2); munit_assert_double(result.m32, ==,  1); munit_assert_double(result.m33, ==,  0);
  return MUNIT_OK;
}

static MunitResult test_matrix_vector_dot(const MunitParameter params[], void *data) {
  vector_t result = matrix_vector_dot(matrix(1, 2, 3, 4, 5, 6, 7, 8, 9), vector(1, 2, 3));
  munit_assert_double(result.x, ==, 14);
  munit_assert_double(result.y, ==, 32);
  munit_assert_double(result.z, ==, 50);
  return MUNIT_OK;
}

static MunitResult test_transpose(const MunitParameter params[], void *data) {
  matrix_t result = transpose(matrix(1, 2, 3, 4, 5, 6, 7, 8, 9));
  munit_assert_double(result.m11, ==, 1); munit_assert_double(result.m12, ==, 4); munit_assert_double(result.m13, ==, 7);
  munit_assert_double(result.m21, ==, 2); munit_assert_double(result.m22, ==, 5); munit_assert_double(result.m23, ==, 8);
  munit_assert_double(result.m31, ==, 3); munit_assert_double(result.m32, ==, 6); munit_assert_double(result.m33, ==, 9);
  return MUNIT_OK;
}

static MunitResult test_matrix_dot(const MunitParameter params[], void *data) {
  matrix_t result = matrix_dot(matrix(1, 2, 3, 4, 5, 6, 7, 8, 9), matrix(10, 11, 12, 13, 14, 15, 16, 17, 18));
  munit_assert_double(result.m11, ==,  84); munit_assert_double(result.m12, ==,  90); munit_assert_double(result.m13, ==,  96);
  munit_assert_double(result.m21, ==, 201); munit_assert_double(result.m22, ==, 216); munit_assert_double(result.m23, ==, 231);
  munit_assert_double(result.m31, ==, 318); munit_assert_double(result.m32, ==, 342); munit_assert_double(result.m33, ==, 366);
  return MUNIT_OK;
}

MunitTest test_matrix[] = {
  {"/create"              , test_create              , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cross_product_matrix", test_cross_product_matrix, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/matrix_vector_dot"   , test_matrix_vector_dot   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/transpose"           , test_transpose           , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/matrix_dot"          , test_matrix_dot          , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                   , NULL                     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
