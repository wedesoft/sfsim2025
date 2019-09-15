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

static MunitResult test_add_point(const MunitParameter params[], void *data)
{
  rigid_body_t *body = make_rigid_body();
  add_point(body, point(2, 3, 5));
  munit_assert_int(body->points->size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_add_face(const MunitParameter params[], void *data)
{
  rigid_body_t *body = make_rigid_body();
  add_face(body, face(0, 1, 2));
  munit_assert_int(body->faces->size, ==, 1);
  return MUNIT_OK;
}

MunitTest test_rigid_body[] = {
  {"/create"   , test_create   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_point", test_add_point, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_face" , test_add_face , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL        , NULL          , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
