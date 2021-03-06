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
  vector_t v[3] = {vector(1, 0, 0), vector(0, 1, 0), vector(0, 0, 1)};
  for (int j=0; j<3; j++) {
    vector_t a = v[j];
    for (int i=0; i<3; i++) {
      vector_t b = v[i];
      munit_assert_true(vector_eq(matrix_vector_dot(cross_product_matrix(a), b), cross_product(a, b)));
    };
  };
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

static MunitResult test_inverse(const MunitParameter params[], void *data) {
  matrix_t m = matrix(2, 3, 5, 7, 11, 13, 17, 19, 23);
  matrix_t result = matrix_dot(m, inverse(m));
  munit_assert_double_equal(result.m11, 1, 6);
  munit_assert_double_equal(result.m12, 0, 6);
  munit_assert_double_equal(result.m13, 0, 6);
  munit_assert_double_equal(result.m21, 0, 6);
  munit_assert_double_equal(result.m22, 1, 6);
  munit_assert_double_equal(result.m23, 0, 6);
  munit_assert_double_equal(result.m31, 0, 6);
  munit_assert_double_equal(result.m32, 0, 6);
  munit_assert_double_equal(result.m33, 1, 6);
  return MUNIT_OK;
}

static MunitResult test_diagonal(const MunitParameter params[], void *data) {
  matrix_t result = diagonal(1, 2, 3);
  munit_assert_double(result.m11, ==, 1); munit_assert_double(result.m12, ==, 0); munit_assert_double(result.m13, ==, 0);
  munit_assert_double(result.m21, ==, 0); munit_assert_double(result.m22, ==, 2); munit_assert_double(result.m23, ==, 0);
  munit_assert_double(result.m31, ==, 0); munit_assert_double(result.m32, ==, 0); munit_assert_double(result.m33, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_identity(const MunitParameter params[], void *data) {
  matrix_t result = identity();
  munit_assert_double(result.m11, ==, 1); munit_assert_double(result.m12, ==, 0); munit_assert_double(result.m13, ==, 0);
  munit_assert_double(result.m21, ==, 0); munit_assert_double(result.m22, ==, 1); munit_assert_double(result.m23, ==, 0);
  munit_assert_double(result.m31, ==, 0); munit_assert_double(result.m32, ==, 0); munit_assert_double(result.m33, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_scale(const MunitParameter params[], void *data) {
  matrix_t result = matrix_scale(matrix(1, 2, 3, 4, 5, 6, 7, 8, 9), 2);
  munit_assert_double(result.m11, ==,  2); munit_assert_double(result.m12, ==,  4); munit_assert_double(result.m13, ==,  6);
  munit_assert_double(result.m21, ==,  8); munit_assert_double(result.m22, ==, 10); munit_assert_double(result.m23, ==, 12);
  munit_assert_double(result.m31, ==, 14); munit_assert_double(result.m32, ==, 16); munit_assert_double(result.m33, ==, 18);
  return MUNIT_OK;
}

static MunitResult test_negative(const MunitParameter params[], void *data) {
  matrix_t result = matrix_negative(matrix(1, 2, 3, 4, 5, 6, 7, 8, 9));
  munit_assert_double(result.m11, ==, -1); munit_assert_double(result.m12, ==, -2); munit_assert_double(result.m13, ==, -3);
  munit_assert_double(result.m21, ==, -4); munit_assert_double(result.m22, ==, -5); munit_assert_double(result.m23, ==, -6);
  munit_assert_double(result.m31, ==, -7); munit_assert_double(result.m32, ==, -8); munit_assert_double(result.m33, ==, -9);
  return MUNIT_OK;
}

static MunitResult test_rotation_x(const MunitParameter params[], void *data) {
  float ca = sqrt(3) / 2;
  float sa = 0.5;
  matrix_t rot = rotation_x(M_PI / 6);
  munit_assert_double_equal(rot.m11, 1, 6); munit_assert_double_equal(rot.m12,  0, 6); munit_assert_double_equal(rot.m13,   0, 6);
  munit_assert_double_equal(rot.m21, 0, 6); munit_assert_double_equal(rot.m22, ca, 6); munit_assert_double_equal(rot.m23, -sa, 6);
  munit_assert_double_equal(rot.m31, 0, 6); munit_assert_double_equal(rot.m32, sa, 6); munit_assert_double_equal(rot.m33,  ca, 6);
  return MUNIT_OK;
}

static MunitResult test_rotation_y(const MunitParameter params[], void *data) {
  float ca = sqrt(3) / 2;
  float sa = 0.5;
  matrix_t rot = rotation_y(M_PI / 6);
  munit_assert_double_equal(rot.m11,  ca, 6); munit_assert_double_equal(rot.m12, 0, 6); munit_assert_double_equal(rot.m13, sa, 6);
  munit_assert_double_equal(rot.m21,   0, 6); munit_assert_double_equal(rot.m22, 1, 6); munit_assert_double_equal(rot.m23,  0, 6);
  munit_assert_double_equal(rot.m31, -sa, 6); munit_assert_double_equal(rot.m32, 0, 6); munit_assert_double_equal(rot.m33, ca, 6);
  return MUNIT_OK;
}

static MunitResult test_rotation_z(const MunitParameter params[], void *data) {
  float ca = sqrt(3) / 2;
  float sa = 0.5;
  matrix_t rot = rotation_z(M_PI / 6);
  munit_assert_double_equal(rot.m11, ca, 6); munit_assert_double_equal(rot.m12, -sa, 6); munit_assert_double_equal(rot.m13, 0, 6);
  munit_assert_double_equal(rot.m21, sa, 6); munit_assert_double_equal(rot.m22,  ca, 6); munit_assert_double_equal(rot.m23, 0, 6);
  munit_assert_double_equal(rot.m31,  0, 6); munit_assert_double_equal(rot.m32,   0, 6); munit_assert_double_equal(rot.m33, 1, 6);
  return MUNIT_OK;
}

MunitTest test_matrix[] = {
  {"/create"              , test_create              , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cross_product_matrix", test_cross_product_matrix, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/matrix_vector_dot"   , test_matrix_vector_dot   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/transpose"           , test_transpose           , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/matrix_dot"          , test_matrix_dot          , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/inverse"             , test_inverse             , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/diagonal"            , test_diagonal            , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/identity"            , test_identity            , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale"               , test_scale               , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/negative"            , test_negative            , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotation_x"          , test_rotation_x          , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotation_y"          , test_rotation_y          , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotation_z"          , test_rotation_z          , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                   , NULL                     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
