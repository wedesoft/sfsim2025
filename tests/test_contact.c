#include "sfsim2025/contact.h"
#include "test_contact.h"


static MunitResult test_body_indices(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1);
  munit_assert_int(c.i, ==, 2);
  munit_assert_int(c.j, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_normal(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1);
  munit_assert_double(c.normal.x, ==,  5);
  munit_assert_double(c.normal.y, ==,  7);
  munit_assert_double(c.normal.z, ==, 11);
  return MUNIT_OK;
}

static MunitResult test_point(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1);
  munit_assert_double(c.point.x, ==, 1);
  munit_assert_double(c.point.y, ==, 2);
  munit_assert_double(c.point.z, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_distance(const MunitParameter paramsp[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1);
  munit_assert_double(c.distance, ==, 0.1);
  return MUNIT_OK;
}

MunitTest test_contact[] = {
  {"/body_indices" , test_body_indices , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/normal"       , test_normal       , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/point"        , test_point        , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/distance"     , test_distance     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL            , NULL              , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
