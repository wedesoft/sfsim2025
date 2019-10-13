#include "sfsim2025/matrix43.h"
#include "sfsim2025/vector.h"
#include "test_matrix43.h"


static MunitResult test_create(const MunitParameter params[], void *data) {
  matrix43_t result = matrix43(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
  munit_assert_double(result.m11, ==,  1); munit_assert_double(result.m12, ==,  2); munit_assert_double(result.m13, ==,  3);
  munit_assert_double(result.m21, ==,  4); munit_assert_double(result.m22, ==,  5); munit_assert_double(result.m23, ==,  6);
  munit_assert_double(result.m31, ==,  7); munit_assert_double(result.m32, ==,  8); munit_assert_double(result.m33, ==,  9);
  munit_assert_double(result.m41, ==, 10); munit_assert_double(result.m42, ==, 11); munit_assert_double(result.m43, ==, 12);
  return MUNIT_OK;
}

static MunitResult test_matrix_vector_dot(const MunitParameter params[], void *data) {
  quaternion_t result = matrix43_vector_dot(matrix43(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12), vector(1, 2, 3));
  munit_assert_double(result.a, ==, 14);
  munit_assert_double(result.b, ==, 32);
  munit_assert_double(result.c, ==, 50);
  munit_assert_double(result.d, ==, 68);
  return MUNIT_OK;
}

static MunitResult test_quaternion(const MunitParameter params[], void *data) {
  vector_t v[3] = {vector(1, 0, 0), vector(0, 1, 0), vector(0, 0, 1)};
  quaternion_t q[4] = {quaternion(1, 0, 0, 0), quaternion(0, 1, 0, 0), quaternion(0, 0, 1, 0), quaternion(0, 0, 0, 1)};
  for (int j=0; j<3; j++) {
    vector_t a = v[j];
    for (int i=0; i<4; i++) {
      quaternion_t b = q[i];
      quaternion_t p1 = quaternion_product(vector_to_quaternion(vector_scale(a, 0.5)), b);
      quaternion_t p2 = matrix43_vector_dot(quaternion_matrix(b), a);
      munit_assert_true(quaternion_eq(p1, p2));
    };
  };
  return MUNIT_OK;
}

MunitTest test_matrix43[] = {
  {"/create"           , test_create           , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/matrix_vector_dot", test_matrix_vector_dot, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/quaternion"       , test_quaternion       , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                , NULL                  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
