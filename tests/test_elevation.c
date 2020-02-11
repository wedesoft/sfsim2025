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

static MunitResult test_no_data(const MunitParameter params[], void *data) {
  elevation_t elevation = read_elevation("nosuchfile.raw");
  munit_assert_int(elevation.height, ==, 0);
  munit_assert_int(elevation.width, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_read_data(const MunitParameter params[], void *data) {
  elevation_t elevation = read_elevation("data.raw");
  munit_assert_int(elevation.height, ==, 1);
  munit_assert_int(elevation.width, ==, 1);
  munit_assert_int(elevation.data[0], ==, -500);
  return MUNIT_OK;
}

static MunitResult test_write_data(const MunitParameter params[], void *data) {
  elevation_t elevation = read_elevation("data.raw");
  write_elevation(elevation, "/tmp/test-elevation-write.raw");
  elevation_t loaded = read_elevation("/tmp/test-elevation-write.raw");
  munit_assert_int(loaded.height, ==, 1);
  munit_assert_int(loaded.width, ==, 1);
  munit_assert_int(loaded.data[0], ==, -500);
  return MUNIT_OK;
}

MunitTest test_elevation[] = {
  {"/shape"      , test_shape      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_data"    , test_no_data    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/read_data"  , test_read_data  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/write_data" , test_write_data , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL          , NULL            , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
