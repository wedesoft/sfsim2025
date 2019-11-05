#include "sfsim2025/world.h"
#include "test_world.h"
#include "test_helper.h"


static MunitResult test_create(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  munit_assert_not_null(world);
  return MUNIT_OK;
}


MunitTest test_world[] = {
  {"/create", test_create, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL     , NULL       , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
