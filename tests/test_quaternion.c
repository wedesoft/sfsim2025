#include "sfsim2025/quaternion.h"
#include "test_quaternion.h"


static MunitResult test_initialize(const MunitParameter params[], void *data) {
  quaternion_t q = quaternion(2, 3, 5, 7);
  munit_assert_double(q.a, ==, 2.0);
  munit_assert_double(q.b, ==, 3.0);
  munit_assert_double(q.c, ==, 5.0);
  munit_assert_double(q.d, ==, 7.0);
  return MUNIT_OK;
}

static MunitResult test_eq(const MunitParameter params[], void *data) {
  munit_assert_true(quaternion_eq(quaternion(2, 3, 5, 7), quaternion(2, 3, 5, 7)));
  munit_assert_false(quaternion_eq(quaternion(1, 3, 5, 7), quaternion(2, 3, 5, 7)));
  munit_assert_false(quaternion_eq(quaternion(2, 2, 5, 7), quaternion(2, 3, 5, 7)));
  munit_assert_false(quaternion_eq(quaternion(2, 3, 4, 7), quaternion(2, 3, 5, 7)));
  munit_assert_false(quaternion_eq(quaternion(2, 3, 5, 6), quaternion(2, 3, 5, 7)));
  return MUNIT_OK;
}

static MunitResult test_from_vector(const MunitParameter params[], void *data) {
  quaternion_t q = vector_to_quaternion(vector(2, 3, 5));
  munit_assert_double(q.a, ==, 0);
  munit_assert_double(q.b, ==, 2);
  munit_assert_double(q.c, ==, 3);
  munit_assert_double(q.d, ==, 5);
  return MUNIT_OK;
}

static MunitResult test_to_vector(const MunitParameter params[], void *data) {
  vector_t v = quaternion_to_vector(quaternion(2, 3, 5, 7));
  munit_assert_double(v.x, ==, 3);
  munit_assert_double(v.y, ==, 5);
  munit_assert_double(v.z, ==, 7);
  return MUNIT_OK;
}

static MunitResult test_sinc(const MunitParameter params[], void *data) {
  munit_assert_double_equal(sinc(0), 1, 6);
  munit_assert_double_equal(sinc(M_PI), 0, 6);
  return MUNIT_OK;
}

static MunitResult test_exp_real(const MunitParameter params[], void *data) {
  munit_assert_double_equal(quaternion_exp(quaternion(1, 0, 0, 0)).a, exp(1), 6);
  munit_assert_double_equal(quaternion_exp(quaternion(1, M_PI, 0, 0)).a, -exp(1), 6);
  return MUNIT_OK;
}

static MunitResult test_exp_imag(const MunitParameter params[], void *data) {
  munit_assert_double_equal(quaternion_exp(quaternion(1, M_PI / 2, 0, 0)).b, exp(1), 6);
  munit_assert_double_equal(quaternion_exp(quaternion(1, 0, M_PI / 2, 0)).c, exp(1), 6);
  munit_assert_double_equal(quaternion_exp(quaternion(1, 0, 0, M_PI / 2)).d, exp(1), 6);
  return MUNIT_OK;
}

static MunitResult test_no_rotation(const MunitParameter params[], void *data) {
  quaternion_t q = quaternion_rotation(0, vector(1, 0, 0));
  munit_assert_double_equal(q.a, 1, 6);
  munit_assert_double_equal(q.b, 0, 6);
  munit_assert_double_equal(q.c, 0, 6);
  munit_assert_double_equal(q.d, 0, 6);
  return MUNIT_OK;
}

static MunitResult test_rotation(const MunitParameter params[], void *data) {
  quaternion_t q = quaternion_rotation(M_PI, vector(1, 0, 0));
  munit_assert_double_equal(q.a, 0, 6);
  munit_assert_double_equal(q.b, 1, 6);
  munit_assert_double_equal(q.c, 0, 6);
  munit_assert_double_equal(q.d, 0, 6);
  return MUNIT_OK;
}

static MunitResult test_conjugate(const MunitParameter params[], void *data) {
  munit_assert_double(quaternion_conjugate(quaternion(1, 0, 0, 0)).a, ==, 1);
  munit_assert_double(quaternion_conjugate(quaternion(2, 0, 0, 0)).a, ==, 0.5);
  munit_assert_double(quaternion_conjugate(quaternion(0, 2, 0, 0)).b, ==, -0.5);
  munit_assert_double(quaternion_conjugate(quaternion(0, 0, 2, 0)).c, ==, -0.5);
  munit_assert_double(quaternion_conjugate(quaternion(0, 0, 0, 2)).d, ==, -0.5);
  return MUNIT_OK;
}

static MunitResult test_product(const MunitParameter params[], void *data) {
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(2, 0, 0, 0), quaternion(2, 0, 0, 0)), quaternion( 4,  0,  0,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(2, 0, 0, 0), quaternion(0, 1, 0, 0)), quaternion( 0,  2,  0,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(2, 0, 0, 0), quaternion(0, 0, 1, 0)), quaternion( 0,  0,  2,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(2, 0, 0, 0), quaternion(0, 0, 0, 1)), quaternion( 0,  0,  0,  2)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 1, 0, 0), quaternion(2, 0, 0, 0)), quaternion( 0,  2,  0,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 1, 0, 0), quaternion(0, 1, 0, 0)), quaternion(-1,  0,  0,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 1, 0, 0), quaternion(0, 0, 1, 0)), quaternion( 0,  0,  0,  1)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 1, 0, 0), quaternion(0, 0, 0, 1)), quaternion( 0,  0, -1,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 0, 1, 0), quaternion(2, 0, 0, 0)), quaternion( 0,  0,  2,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 0, 1, 0), quaternion(0, 1, 0, 0)), quaternion( 0,  0,  0, -1)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 0, 1, 0), quaternion(0, 0, 1, 0)), quaternion(-1,  0,  0,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 0, 1, 0), quaternion(0, 0, 0, 1)), quaternion( 0,  1,  0,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 0, 0, 1), quaternion(2, 0, 0, 0)), quaternion( 0,  0,  0,  2)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 0, 0, 1), quaternion(0, 1, 0, 0)), quaternion( 0,  0,  1,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 0, 0, 1), quaternion(0, 0, 1, 0)), quaternion( 0, -1,  0,  0)));
  munit_assert_true(quaternion_eq(quaternion_product(quaternion(0, 0, 0, 1), quaternion(0, 0, 0, 1)), quaternion(-1,  0,  0,  0)));
  return MUNIT_OK;
}

static MunitResult test_dont_rotate(const MunitParameter params[], void *data) {
  vector_t v = rotate_vector(quaternion_rotation(0, vector(1, 0, 0)), vector(2, 4, 8));
  munit_assert_double_equal(v.x, 2, 6);
  munit_assert_double_equal(v.y, 4, 6);
  munit_assert_double_equal(v.z, 8, 6);
  return MUNIT_OK;
}

static MunitResult test_rotate_vector(const MunitParameter params[], void *data) {
  vector_t v = rotate_vector(quaternion_rotation(M_PI / 2, vector(1, 0, 0)), vector(2, 4, 8));
  munit_assert_double_equal(v.x,  2, 6);
  munit_assert_double_equal(v.y, -8, 6);
  munit_assert_double_equal(v.z,  4, 6);
  return MUNIT_OK;
}

static MunitResult test_identity_matrix(const MunitParameter params[], void *data) {
  matrix_t result = rotation_matrix(quaternion_rotation(0, vector(1, 0, 0)));
  munit_assert_double(result.m11, ==, 1); munit_assert_double(result.m12, ==, 0); munit_assert_double(result.m13, ==, 0);
  munit_assert_double(result.m21, ==, 0); munit_assert_double(result.m22, ==, 1); munit_assert_double(result.m23, ==, 0);
  munit_assert_double(result.m31, ==, 0); munit_assert_double(result.m32, ==, 0); munit_assert_double(result.m33, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_rotation_matrix(const MunitParameter params[], void *data) {
  matrix_t m = rotation_matrix(quaternion_rotation(M_PI / 2, vector(0, 0, 1)));
  munit_assert_double_equal(m.m11, 0, 6); munit_assert_double_equal(m.m12, -1, 6); munit_assert_double_equal(m.m13, 0, 6);
  munit_assert_double_equal(m.m21, 1, 6); munit_assert_double_equal(m.m22,  0, 6); munit_assert_double_equal(m.m23, 0, 6);
  munit_assert_double_equal(m.m31, 0, 6); munit_assert_double_equal(m.m32,  0, 6); munit_assert_double_equal(m.m33, 1, 6);
  return MUNIT_OK;
}

static MunitResult test_no_rotate(const MunitParameter params[], void *data) {
  matrix_t m = rotate_matrix(quaternion_rotation(0, vector(0, 0, 1)), matrix(1, 2, 3, 4, 5, 6, 7, 8, 9));
  munit_assert_double_equal(m.m11, 1, 6); munit_assert_double_equal(m.m12, 2, 6); munit_assert_double_equal(m.m13, 3, 6);
  munit_assert_double_equal(m.m21, 4, 6); munit_assert_double_equal(m.m22, 5, 6); munit_assert_double_equal(m.m23, 6, 6);
  munit_assert_double_equal(m.m31, 7, 6); munit_assert_double_equal(m.m32, 8, 6); munit_assert_double_equal(m.m33, 9, 6);
  return MUNIT_OK;
}

static MunitResult test_rotate_matrix(const MunitParameter params[], void *data) {
  matrix_t m = rotate_matrix(quaternion_rotation(M_PI / 2, vector(0, 0, 1)), matrix(1, 2, 3, 4, 5, 6, 7, 8, 9));
  munit_assert_double_equal(m.m11,  5, 6); munit_assert_double_equal(m.m12, -4, 6); munit_assert_double_equal(m.m13, -6, 6);
  munit_assert_double_equal(m.m21, -2, 6); munit_assert_double_equal(m.m22,  1, 6); munit_assert_double_equal(m.m23,  3, 6);
  munit_assert_double_equal(m.m31, -8, 6); munit_assert_double_equal(m.m32,  7, 6); munit_assert_double_equal(m.m33,  9, 6);
  return MUNIT_OK;
}

MunitTest test_quaternion[] = {
  {"/initialize"     , test_initialize     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/eq"             , test_eq             , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/from_vector"    , test_from_vector    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/to_vector"      , test_to_vector      , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/sinc"           , test_sinc           , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/exp_real"       , test_exp_real       , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/exp_imag"       , test_exp_imag       , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_rotation"    , test_no_rotation    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotation"       , test_rotation       , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/conjugate"      , test_conjugate      , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/product"        , test_product        , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/dont_rotate"    , test_dont_rotate    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotate_vector"  , test_rotate_vector  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/identity_matrix", test_identity_matrix, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotation_matrix", test_rotation_matrix, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_rotate"      , test_no_rotate      , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotate_matrix"  , test_rotate_matrix  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL              , NULL                , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
