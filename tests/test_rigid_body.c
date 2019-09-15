#include "sfsim2025/rigid_body.h"
#include "test_rigid_body.h"
#include "test_helper.h"


static MunitResult test_create(const MunitParameter params[], void *data)
{
  munit_assert_int(make_rigid_body()->points->size, ==, 0);
  munit_assert_int(make_rigid_body()->edges->size, ==, 0);
  munit_assert_int(make_rigid_body()->faces->size, ==, 0);
  return MUNIT_OK;
}

MunitTest test_rigid_body[] = {
  {"/create", test_create, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL     , NULL       , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
