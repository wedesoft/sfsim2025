#include "sfsim2025/vector.h"
#include "test_vector.h"
#include "test_helper.h"


static MunitResult test_difference(const MunitParameter params[], void *data)
{
  vector_t result = difference(vector(7, 11, 13), vector(2, 3, 5));
  munit_assert_double(result.x, ==, 5.0);
  munit_assert_double(result.y, ==, 8.0);
  munit_assert_double(result.z, ==, 8.0);
  return MUNIT_OK;
}

static MunitResult test_cross_product(const MunitParameter params[], void *data)
{
  vector_t result = cross_product(vector(1, 0, 0), vector(0, 1, 0));
  munit_assert_double(result.x, ==, 0.0);
  munit_assert_double(result.y, ==, 0.0);
  munit_assert_double(result.z, ==, 1.0);
  return MUNIT_OK;
}

static MunitResult test_normalize(const MunitParameter params[], void *data)
{
  vector_t result = normalize(vector(3, 4, 0));
  munit_assert_double(result.x, ==, 0.6);
  munit_assert_double(result.y, ==, 0.8);
  munit_assert_double(result.z, ==, 0.0);
  return MUNIT_OK;
}

MunitTest test_vector[] = {
  {"/difference"   , test_difference   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cross_product", test_cross_product, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/normalize"    , test_normalize    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL            , NULL              , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
