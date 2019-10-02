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

static MunitResult test_from_vector(const MunitParameter params[], void *data) {
  quaternion_t q = vector_to_quaternion(vector(2, 3, 5));
  munit_assert_double(q.a, ==, 0);
  munit_assert_double(q.b, ==, 2);
  munit_assert_double(q.c, ==, 3);
  munit_assert_double(q.d, ==, 5);
  return MUNIT_OK;
}

MunitTest test_quaternion[] = {
  {"/initialize" , test_initialize  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/from_vector", test_from_vector , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL          , NULL             , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
