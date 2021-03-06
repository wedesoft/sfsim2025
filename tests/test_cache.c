#include "sfsim2025/cache.h"
#include "test_cache.h"
#include "test_helper.h"


static MunitResult test_init(const MunitParameter params[], void *data) {
  cache_t cache = make_cache(2);
  munit_assert_int(cache.size, ==, 2);
  munit_assert_int(cache.keys.size, ==, 0);
  munit_assert_int(cache.values.size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_image_cache_miss(const MunitParameter params[], void *data) {
  cache_t cache = make_cache(2);
  image_t image = cache_image(&cache, "cache-%d-%d-%d.png", 2, 3, 5);
  munit_assert_int(cache.keys.size, ==, 1);
  munit_assert_int(cache.values.size, ==, 1);
  munit_assert_int(image.data[0], ==, 2);
  return MUNIT_OK;
}

static MunitResult test_drop_image(const MunitParameter params[], void *data) {
  cache_t cache = make_cache(2);
  cache_image(&cache, "cache-%d-%d-%d.png", 2, 3, 5);
  cache_image(&cache, "cache-%d-%d-%d.png", 2, 4, 5);
  cache_image(&cache, "cache-%d-%d-%d.png", 2, 7, 5);
  munit_assert_int(cache.keys.size, ==, 2);
  munit_assert_int(cache.values.size, ==, 2);
  munit_assert_int(get_image(cache.values)[0].data[0], ==, 3);
  munit_assert_int(get_image(cache.values)[1].data[0], ==, 5);
  return MUNIT_OK;
}

static MunitResult test_image_cache_hit(const MunitParameter params[], void *data) {
  cache_t cache = make_cache(2);
  cache_image(&cache, "cache-%d-%d-%d.png", 2, 3, 5);
  image_t image = cache_image(&cache, "cache-%d-%d-%d.png", 2, 3, 5);
  munit_assert_int(cache.keys.size, ==, 1);
  munit_assert_int(cache.values.size, ==, 1);
  munit_assert_int(image.data[0], ==, 2);
  return MUNIT_OK;
}

static MunitResult test_elevation_cache_miss(const MunitParameter params[], void *data) {
  cache_t cache = make_cache(2);
  elevation_t elevation = cache_elevation(&cache, "cache-%d-%d-%d.raw", 2, 3, 5);
  munit_assert_int(cache.keys.size, ==, 1);
  munit_assert_int(cache.values.size, ==, 1);
  munit_assert_int(elevation.data[0], ==, 2);
  return MUNIT_OK;
}

static MunitResult test_drop_elevation(const MunitParameter params[], void *data) {
  cache_t cache = make_cache(2);
  cache_elevation(&cache, "cache-%d-%d-%d.raw", 2, 3, 5);
  cache_elevation(&cache, "cache-%d-%d-%d.raw", 2, 4, 5);
  cache_elevation(&cache, "cache-%d-%d-%d.raw", 2, 7, 5);
  munit_assert_int(cache.keys.size, ==, 2);
  munit_assert_int(cache.values.size, ==, 2);
  munit_assert_int(get_elevation(cache.values)[0].data[0], ==, 3);
  munit_assert_int(get_elevation(cache.values)[1].data[0], ==, 5);
  return MUNIT_OK;
}

static MunitResult test_elevation_cache_hit(const MunitParameter params[], void *data) {
  cache_t cache = make_cache(2);
  cache_elevation(&cache, "cache-%d-%d-%d.raw", 2, 3, 5);
  elevation_t elevation = cache_elevation(&cache, "cache-%d-%d-%d.raw", 2, 3, 5);
  munit_assert_int(cache.keys.size, ==, 1);
  munit_assert_int(cache.values.size, ==, 1);
  munit_assert_int(elevation.data[0], ==, 2);
  return MUNIT_OK;
}

MunitTest test_cache[] = {
  {"/init"                , test_init                , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/image_cache_miss"    , test_image_cache_miss    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/drop_image"          , test_drop_image          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/image_cache_hit"     , test_image_cache_hit     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/elevation_cache_miss", test_elevation_cache_miss, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/drop_elevation"      , test_drop_elevation      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/elevation_cache_hit" , test_elevation_cache_hit , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                   , NULL                     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
