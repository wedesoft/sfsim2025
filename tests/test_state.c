#include "sfsim2025/state.h"
#include "test_state.h"
#include "test_helper.h"


static MunitResult test_components(const MunitParameter params[], void *data) {
  state_t *result = state(vector(2, 3, 5), vector(3, 5, 7), quaternion(1, 0, 0, 0), vector(5, 7, 11));
  munit_assert_double(result->position.x, ==, 2);
  munit_assert_double(result->speed.x, ==, 3);
  munit_assert_double(result->orientation.a, ==, 1);
  munit_assert_double(result->rotation.x, ==, 5);
  return MUNIT_OK;
}

MunitTest test_state[] = {
  {"/components", test_components, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL         , NULL           , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
