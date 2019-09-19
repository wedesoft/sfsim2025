#include "sfsim2025/plane.h"
#include "test_plane.h"
#include "test_helper.h"


static MunitResult test_plane_distance(const MunitParameter params[], void *data) {
  plane_t p = plane(vector(2, 3, 5), vector(0, 0, 1));
  munit_assert_double(plane_distance(p, vector(3, 4, 8)), ==, 3);
  return MUNIT_OK;
}

// Return plane with inverted normal vector.
static MunitResult test_negative_plane(const MunitParameter params[], void *data) {
  plane_t p = plane(vector(2, 3, 5), vector(0, 0, 1));
  munit_assert_double(negative_plane(p).normal.z, ==, -1);
  return MUNIT_OK;
}

MunitTest test_plane[] = {
  {"/plane_distance", test_plane_distance, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/negative_plane", test_negative_plane, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL             , NULL               , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
