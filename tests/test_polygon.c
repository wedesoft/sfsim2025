#include "sfsim2025/polygon.h"
#include "test_polygon.h"
#include "test_helper.h"


static list_t *make_polygon(void) {
  list_t *result = make_list();
  append_coordinate(result, coordinate( 0, 3));
  append_coordinate(result, coordinate( 2, 0));
  append_coordinate(result, coordinate(-2, 0));
  return result;
}

static list_t *make_polygon2(void) {
  list_t *result = make_list();
  append_coordinate(result, coordinate( 0, 3));
  append_coordinate(result, coordinate( 0, 1));
  append_coordinate(result, coordinate( 2, 0));
  append_coordinate(result, coordinate(-2, 0));
  return result;
}

static list_t *make_polygon3(void) {
  list_t *result = make_list();
  append_coordinate(result, coordinate( 0, 2));
  append_coordinate(result, coordinate( 2, 0));
  append_coordinate(result, coordinate( 0, 0));
  return result;
}

static list_t *make_polygon4(void) {
  list_t *result = make_list();
  append_coordinate(result, coordinate(0, 2));
  append_coordinate(result, coordinate(1, 2));
  append_coordinate(result, coordinate(2, 2));
  append_coordinate(result, coordinate(1, 0));
  return result;
}

static list_t *make_polygon5(void) {
  list_t *result = make_list();
  append_coordinate(result, coordinate(0, 2));
  append_coordinate(result, coordinate(2, 2));
  append_coordinate(result, coordinate(1, 2));
  append_coordinate(result, coordinate(1, 0));
  return result;
}

static MunitResult test_leftmost_point(const MunitParameter params[], void *data) {
  list_t *result = convex_hull(make_polygon());
  munit_assert_double(get_coordinate(result)[0].u, ==, -2);
  munit_assert_double(get_coordinate(result)[0].v, ==,  0);
  return MUNIT_OK;
}

static MunitResult test_next_point(const MunitParameter params[], void *data) {
  list_t *result = convex_hull(make_polygon());
  munit_assert_double(get_coordinate(result)[1].u, ==, 0);
  munit_assert_double(get_coordinate(result)[1].v, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_third_point(const MunitParameter params[], void *data) {
  list_t *result = convex_hull(make_polygon());
  munit_assert_int(result->size, ==, 3);
  munit_assert_double(get_coordinate(result)[2].u, ==, 2);
  munit_assert_double(get_coordinate(result)[2].v, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_skip_point(const MunitParameter params[], void *data) {
  list_t *result = convex_hull(make_polygon2());
  munit_assert_int(result->size, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_bottommost_point(const MunitParameter params[], void *data) {
  list_t *result = convex_hull(make_polygon3());
  munit_assert_double(get_coordinate(result)[0].u, ==, 0);
  munit_assert_double(get_coordinate(result)[0].v, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_skip_collinear(const MunitParameter params[], void *data) {
  list_t *result = convex_hull(make_polygon4());
  munit_assert_int(result->size, ==, 3);
  munit_assert_double(get_coordinate(result)[1].u, ==, 2);
  munit_assert_double(get_coordinate(result)[1].v, ==, 2);
  return MUNIT_OK;
}

static MunitResult test_skip_collinear2(const MunitParameter params[], void *data) {
  list_t *result = convex_hull(make_polygon5());
  munit_assert_int(result->size, ==, 3);
  munit_assert_double(get_coordinate(result)[1].u, ==, 2);
  munit_assert_double(get_coordinate(result)[1].v, ==, 2);
  return MUNIT_OK;
}

MunitTest test_polygon[] = {
  {"/leftmost_point"  , test_leftmost_point  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/next_point"      , test_next_point      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/third_point"     , test_third_point     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/skip_point"      , test_skip_point      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/bottommost_point", test_bottommost_point, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/skip_collinear"  , test_skip_collinear  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/skip_collinear2" , test_skip_collinear2 , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL               , NULL                 , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};

