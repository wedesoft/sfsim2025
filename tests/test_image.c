#include "sfsim2025/image.h"
#include "test_image.h"
#include "test_helper.h"


static MunitResult test_shape(const MunitParameter params[], void *data) {
  image_t image = read_image("black.png");
  munit_assert_int(image.height, ==, 40);
  munit_assert_int(image.width, ==, 64);
  return MUNIT_OK;
}

static MunitResult test_no_image(const MunitParameter params[], void *data) {
  image_t image = read_image("no_such_image.png");
  munit_assert_int(image.height, ==, 0);
  munit_assert_int(image.width, ==, 0);
  munit_assert_null(image.data);
  return MUNIT_OK;
}

static MunitResult test_color(const MunitParameter params[], void *data) {
  image_t image = read_image("color.png");
  munit_assert_int(image.data[0], ==, 2);
  munit_assert_int(image.data[1], ==, 4);
  munit_assert_int(image.data[2], ==, 6);
  return MUNIT_OK;
}

MunitTest test_image[] = {
  {"/shape"   , test_shape   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_image", test_no_image, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/color"   , test_color   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL       , NULL         , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
