#include "sfsim2025/cache.h"
#include "test_cache.h"
#include "test_helper.h"


static MunitResult test_tile_key(const MunitParameter params[], void *data) {
  munit_assert_string_equal(tile_key(1, 2, 3), "1/3/2");
  return MUNIT_OK;
}

MunitTest test_cache[] = {
  {"/tile_key"   , test_tile_key   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL          , NULL            , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
