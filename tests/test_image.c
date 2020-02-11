#include <gc.h>
#include <sys/stat.h>
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
  image_t cropped = crop_image(image, 1, 2, 3, 4);
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

static MunitResult test_mkdir_p(const MunitParameter params[], void *data) {
  const char *path = "/tmp/test-image-mkdir-p/dir/subdir";
  mkdir_p(path);
  struct stat statbuf;
  munit_assert_int(stat(path, &statbuf), ==, 0);
  return MUNIT_OK;
}

static MunitResult test_tilepath(const MunitParameter params[], void *data) {
  munit_assert_string_equal(tilepath("test", 0, 0, 0, ".png"), "test.png");
  munit_assert_string_equal(tilepath("test", 1, 1, 0, ".png"), "test/10.png");
  munit_assert_string_equal(tilepath("test", 2, 3, 1, ".png"), "test/10/11.png");
  return MUNIT_OK;
}

static MunitResult test_scale_image(const MunitParameter params[], void *data) {
  char *pixels = GC_MALLOC_ATOMIC(12);
  image_t image = (image_t){.height = 2, .width = 2, .data = pixels};
  pixels[0] =   4; pixels[3] =   6; pixels[6] =   8; pixels[ 9] =  10;
  pixels[1] = 104; pixels[4] = 106; pixels[7] = 108; pixels[10] = 110;
  pixels[2] = 204; pixels[5] = 206; pixels[8] = 208; pixels[11] = 210;
  image_t result = scale_image(image);
  munit_assert_int(result.height, ==, 1);
  munit_assert_int(result.width, ==, 1);
  munit_assert_int(result.data[0], ==,   7);
  munit_assert_int(result.data[1], ==, 107);
  munit_assert_int(result.data[2], ==, 207);
  return MUNIT_OK;
}

MunitTest test_image[] = {
  {"/shape"      , test_shape      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_image"   , test_no_image   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/color"      , test_color      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/crop"       , test_crop       , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/write"      , test_write      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/mkdir_p"    , test_mkdir_p    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/tilepath"   , test_tilepath   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_image", test_scale_image, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL          , NULL            , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
