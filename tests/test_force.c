#include "sfsim2025/force.h"
#include "test_force.h"


static MunitResult test_gravitation(const MunitParameter params[], void *data) {
  force_t f = gravitation(1, 2);
  munit_assert_int(f.force_type, ==, GRAVITATION);
  munit_assert_int(f.i, ==, 1);
  munit_assert_int(f.j, ==, 2);
  return MUNIT_OK;
}

MunitTest test_force[] = {
  {"/gravitation", test_gravitation, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL          , NULL            , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
