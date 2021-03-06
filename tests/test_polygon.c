#include "sfsim2025/polygon.h"
#include "test_polygon.h"
#include "test_helper.h"


static list_t triangle(void) {
  list_t result = make_list();
  append_coordinate(&result, coordinate( 0, 3));
  append_coordinate(&result, coordinate(-2, 0));
  append_coordinate(&result, coordinate( 2, 0));
  return result;
}

static list_t large_triangle(void) {
  list_t result = make_list();
  append_coordinate(&result, coordinate( 0,  6));
  append_coordinate(&result, coordinate(-4, -1));
  append_coordinate(&result, coordinate( 4, -1));
  return result;
}

static list_t concave(void) {
  list_t result = make_list();
  append_coordinate(&result, coordinate( 0, 3));
  append_coordinate(&result, coordinate( 0, 1));
  append_coordinate(&result, coordinate(-2, 0));
  append_coordinate(&result, coordinate( 2, 0));
  return result;
}

static list_t left_conflict(void) {
  list_t result = make_list();
  append_coordinate(&result, coordinate( 2, 0));
  append_coordinate(&result, coordinate( 0, 2));
  append_coordinate(&result, coordinate( 0, 0));
  return result;
}

static list_t collinear_points(void) {
  list_t result = make_list();
  append_coordinate(&result, coordinate(0, 2));
  append_coordinate(&result, coordinate(1, 2));
  append_coordinate(&result, coordinate(2, 2));
  append_coordinate(&result, coordinate(1, 3));
  return result;
}

static list_t collinear_points2(void) {
  list_t result = make_list();
  append_coordinate(&result, coordinate(0, 2));
  append_coordinate(&result, coordinate(2, 2));
  append_coordinate(&result, coordinate(1, 2));
  append_coordinate(&result, coordinate(1, 3));
  return result;
}

static list_t square(void) {
  list_t result = make_list();
  append_coordinate(&result, coordinate(0, 2));
  append_coordinate(&result, coordinate(0, 0));
  append_coordinate(&result, coordinate(2, 0));
  append_coordinate(&result, coordinate(2, 2));
  return result;
}

static list_t square2(void) {
  list_t result = make_list();
  append_coordinate(&result, coordinate(1, 3));
  append_coordinate(&result, coordinate(1, 1));
  append_coordinate(&result, coordinate(3, 1));
  append_coordinate(&result, coordinate(3, 3));
  return result;
}

static MunitResult test_leftmost_point(const MunitParameter params[], void *data) {
  list_t result = convex_hull(triangle());
  munit_assert_double(get_coordinate(result)[0].u, ==, -2);
  munit_assert_double(get_coordinate(result)[0].v, ==,  0);
  return MUNIT_OK;
}

static MunitResult test_next_point(const MunitParameter params[], void *data) {
  list_t result = convex_hull(triangle());
  munit_assert_double(get_coordinate(result)[1].u, ==, 2);
  munit_assert_double(get_coordinate(result)[1].v, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_third_point(const MunitParameter params[], void *data) {
  list_t result = convex_hull(triangle());
  munit_assert_int(result.size, ==, 3);
  munit_assert_double(get_coordinate(result)[2].u, ==, 0);
  munit_assert_double(get_coordinate(result)[2].v, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_skip_point(const MunitParameter params[], void *data) {
  list_t result = convex_hull(concave());
  munit_assert_int(result.size, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_topmost_point(const MunitParameter params[], void *data) {
  list_t result = convex_hull(left_conflict());
  munit_assert_double(get_coordinate(result)[0].u, ==, 0);
  munit_assert_double(get_coordinate(result)[0].v, ==, 2);
  return MUNIT_OK;
}

static MunitResult test_skip_collinear(const MunitParameter params[], void *data) {
  list_t result = convex_hull(collinear_points());
  munit_assert_int(result.size, ==, 3);
  munit_assert_double(get_coordinate(result)[1].u, ==, 2);
  munit_assert_double(get_coordinate(result)[1].v, ==, 2);
  return MUNIT_OK;
}

static MunitResult test_skip_collinear2(const MunitParameter params[], void *data) {
  list_t result = convex_hull(collinear_points2());
  munit_assert_int(result.size, ==, 3);
  munit_assert_double(get_coordinate(result)[1].u, ==, 2);
  munit_assert_double(get_coordinate(result)[1].v, ==, 2);
  return MUNIT_OK;
}

static MunitResult test_empty_hull(const MunitParameter params[], void *data) {
  list_t result = convex_hull(make_list());
  munit_assert_int(result.size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_hull_edge_case(const MunitParameter params[], void *data) {
  list_t result = make_list();
  append_coordinate(&result, coordinate(-1, 0));
  append_coordinate(&result, coordinate(-1, 1e-32));
  append_coordinate(&result, coordinate( 0, 1));
  convex_hull(result);
  return MUNIT_OK;
}

static MunitResult test_inside(const MunitParameter params[], void *data) {
  munit_assert_true(inside(triangle(), coordinate(0, 1)));
  return MUNIT_OK;
}

static MunitResult test_outside(const MunitParameter params[], void *data) {
  munit_assert_false(inside(triangle(), coordinate(2, 1)));
  return MUNIT_OK;
}

static MunitResult test_outside2(const MunitParameter params[], void *data) {
  munit_assert_false(inside(triangle(), coordinate(-2, 1)));
  return MUNIT_OK;
}

static MunitResult test_one_point(const MunitParameter params[], void *data) {
  list_t result = make_list();
  append_coordinate(&result, coordinate(0, 0));
  munit_assert_false(inside(result, coordinate(2, 3)));
  return MUNIT_OK;
}

static MunitResult test_subset(const MunitParameter params[], void *data) {
  list_t result = intersection(triangle(), large_triangle());
  munit_assert_int(result.size, ==, 3);
  munit_assert_double(get_coordinate(result)[0].u, ==,  0);
  munit_assert_double(get_coordinate(result)[0].v, ==,  3);
  munit_assert_double(get_coordinate(result)[1].u, ==, -2);
  munit_assert_double(get_coordinate(result)[1].v, ==,  0);
  munit_assert_double(get_coordinate(result)[2].u, ==,  2);
  munit_assert_double(get_coordinate(result)[2].v, ==,  0);
  return MUNIT_OK;
}

static MunitResult test_subset2(const MunitParameter params[], void *data) {
  list_t result = intersection(large_triangle(), triangle());
  munit_assert_int(result.size, ==, 3);
  munit_assert_double(get_coordinate(result)[0].u, ==,  0);
  munit_assert_double(get_coordinate(result)[0].v, ==,  3);
  munit_assert_double(get_coordinate(result)[1].u, ==, -2);
  munit_assert_double(get_coordinate(result)[1].v, ==,  0);
  munit_assert_double(get_coordinate(result)[2].u, ==,  2);
  munit_assert_double(get_coordinate(result)[2].v, ==,  0);
  return MUNIT_OK;
}

static MunitResult test_intersections(const MunitParameter params[], void *data) {
  list_t result = intersection(square(), square2());
  munit_assert_int(result.size, ==, 4);
  munit_assert_double(get_coordinate(result)[0].u, ==, 2);
  munit_assert_double(get_coordinate(result)[0].v, ==, 2);
  munit_assert_double(get_coordinate(result)[1].u, ==, 1);
  munit_assert_double(get_coordinate(result)[1].v, ==, 1);
  munit_assert_double(get_coordinate(result)[2].u, ==, 2);
  munit_assert_double(get_coordinate(result)[2].v, ==, 1);
  munit_assert_double(get_coordinate(result)[3].u, ==, 1);
  munit_assert_double(get_coordinate(result)[3].v, ==, 2);
  return MUNIT_OK;
}

MunitTest test_polygon[] = {
  {"/leftmost_point"  , test_leftmost_point  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/next_point"      , test_next_point      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/third_point"     , test_third_point     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/skip_point"      , test_skip_point      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/topmost_point"   , test_topmost_point   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/skip_collinear"  , test_skip_collinear  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/skip_collinear2" , test_skip_collinear2 , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/empty_hull"      , test_empty_hull      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/hull_edge_case"  , test_hull_edge_case  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/inside"          , test_inside          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/outside"         , test_outside         , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/outside2"        , test_outside2        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/one_point"       , test_one_point       , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/subset"          , test_subset          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/subset2"         , test_subset2         , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/intersections"   , test_intersections   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL               , NULL                 , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};

