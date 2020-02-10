#include <gc.h>
#include <sys/stat.h>
#include "sfsim2025/elevation.h"
#include "test_elevation.h"
#include "test_helper.h"


static MunitResult test_shape(const MunitParameter params[], void *data) {
  elevation_t elevation = read_elevation("zeros.raw");
  munit_assert_int(elevation.height, ==, 3);
  munit_assert_int(elevation.width, ==, 3);
  return MUNIT_OK;
}

MunitTest test_elevation[] = {
  {"/shape"      , test_shape      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL          , NULL            , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
