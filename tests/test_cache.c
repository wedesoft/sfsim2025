#include "sfsim2025/cache.h"
#include "test_cache.h"
#include "test_helper.h"


static MunitResult test_init(const MunitParameter params[], void *data) {
  lru_cache_t cache = make_image_cache(2);
  munit_assert_int(cache.size, ==, 2);
  munit_assert_int(cache.list.size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_add_image(const MunitParameter params[], void *data) {
  lru_cache_t cache = make_image_cache(2);
  image_t image = cache_image(&cache, "cache-%d-%d-%d.png", 2, 3, 5);
  munit_assert_int(cache.list.size, ==, 1);
  munit_assert_int(image.data[0], ==, 0);
  return MUNIT_OK;
}

MunitTest test_cache[] = {
  {"init"     , test_init     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"add_image", test_add_image, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL       , NULL          , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
