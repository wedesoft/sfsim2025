#include "sfsim2025/vector.h"
#include "test_vector.h"
#include "test_helper.h"


static MunitResult test_cross_product(const MunitParameter params[], void *data)
{
  vector_t result = cross_product(vector(1, 0, 0), vector(0, 1, 0));
  munit_assert_float(result.x, ==, 0.0f);
  munit_assert_float(result.y, ==, 0.0f);
  munit_assert_float(result.z, ==, 1.0f);
  return MUNIT_OK;
}

MunitTest test_vector[] = {
  {"/cross_product", test_cross_product, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL            , NULL              , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
