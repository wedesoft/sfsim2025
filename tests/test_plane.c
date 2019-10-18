#include "sfsim2025/plane.h"
#include "test_plane.h"
#include "test_helper.h"


static MunitResult test_plane_distance(const MunitParameter params[], void *data) {
  plane_t p = plane(vector(2, 3, 5), vector(0, 0, 1));
  munit_assert_double(plane_distance(p, vector(3, 4, 8)), ==, 3);
  return MUNIT_OK;
}

static MunitResult test_negative_plane(const MunitParameter params[], void *data) {
  plane_t p = plane(vector(2, 3, 5), vector(0, 0, 1));
  munit_assert_double(negative_plane(p).normal.z, ==, -1);
  return MUNIT_OK;
}

static MunitResult test_plane_coordinates(const MunitParameter params[], void *data) {
  list_t vectors = make_list();
  append_vector(&vectors, vector(1, 5, 8));
  list_t result = plane_coordinates(plane(vector(1, 2, 3), vector(1, 0, 0)), vectors);
  munit_assert_int(result.size, ==, 1);
  munit_assert_double(get_coordinate(result)[0].u, ==, 3);
  munit_assert_double(get_coordinate(result)[0].v, ==, 5);
  return MUNIT_OK;
}

static MunitResult test_plane_points(const MunitParameter params[], void *data) {
  list_t coordinates = make_list();
  append_coordinate(&coordinates, coordinate(3, 5));
  list_t result = plane_points(plane(vector(1, 2, 3), vector(1, 0, 0)), coordinates);
  munit_assert_int(result.size, ==, 1);
  munit_assert_double(get_vector(result)[0].x, ==, 1);
  munit_assert_double(get_vector(result)[0].y, ==, 5);
  munit_assert_double(get_vector(result)[0].z, ==, 8);
  return MUNIT_OK;
}

MunitTest test_plane[] = {
  {"/plane_distance"        , test_plane_distance   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/negative_plane"        , test_negative_plane   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/test_plane_coordinates", test_plane_coordinates, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/test_plane_points"     , test_plane_points     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                     , NULL                  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
