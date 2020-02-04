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

static MunitResult test_crop(const MunitParameter params[], void *data) {
  image_t image = read_image("indices.png");
  image_t cropped = crop(image, 1, 2, 3, 4);
  munit_assert_int(cropped.height, ==, 3);
  munit_assert_int(cropped.width, ==, 4);
  munit_assert_int(cropped.data[0], ==, 12);
  munit_assert_int(cropped.data[12], ==, 22);
  return MUNIT_OK;
}

static MunitResult test_write(const MunitParameter params[], void *data) {
  image_t image = read_image("indices.png");
  write_image(image, "/tmp/test-image-write.png");
  image_t loaded = read_image("/tmp/test-image-write.png");
  munit_assert_int(loaded.height, ==, image.height);
  munit_assert_int(loaded.width, ==, image.width);
  munit_assert_int(loaded.data[0], ==, image.data[0]);
  munit_assert_int(loaded.data[1], ==, image.data[1]);
  munit_assert_int(loaded.data[2], ==, image.data[2]);
  return MUNIT_OK;
}

MunitTest test_image[] = {
  {"/shape"   , test_shape   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_image", test_no_image, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/color"   , test_color   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/crop"    , test_crop    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/write"   , test_write   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL       , NULL         , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
