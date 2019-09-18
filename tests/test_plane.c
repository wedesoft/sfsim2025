#include "sfsim2025/plane.h"
#include "test_plane.h"
#include "test_helper.h"


static MunitResult test_plane_distance(const MunitParameter params[], void *data)
{
  plane_t p = plane(vector(2, 3, 5), vector(0, 0, 1));
  munit_assert_double(plane_distance(p, vector(3, 4, 8)), ==, 3);
  return MUNIT_OK;
}

MunitTest test_plane[] = {
  {"/plane_distance", test_plane_distance, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL             , NULL               , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
