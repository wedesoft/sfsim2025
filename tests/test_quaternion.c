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

MunitTest test_quaternion[] = {
  {"/initialize" , test_initialize  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/eq"         , test_eq          , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/from_vector", test_from_vector , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/to_vector"  , test_to_vector   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/sinc"       , test_sinc        , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/exp_real"   , test_exp_real    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/exp_imag"   , test_exp_imag    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_rotation", test_no_rotation , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotation"   , test_rotation    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/conjugate"  , test_conjugate   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/product"    , test_product     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL          , NULL             , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
