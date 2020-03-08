#include <tgmath.h>
#include "sfsim2025/map.h"
#include "test_map.h"


static MunitResult test_cube_face_0(const MunitParameter params[], void *data) {
  munit_assert_float(cube_map_y(0, 0, 0), ==,  1);
  munit_assert_float(cube_map_x(0, 0, 0), ==, -1);
  munit_assert_float(cube_map_x(0, 0, 1), ==,  1);
  munit_assert_float(cube_map_z(0, 0, 0), ==, -1);
  munit_assert_float(cube_map_z(0, 1, 0), ==,  1);
  return MUNIT_OK;
}

static MunitResult test_cube_face_1(const MunitParameter params[], void *data) {
  munit_assert_float(cube_map_z(1, 0, 0), ==,  1);
  munit_assert_float(cube_map_x(1, 0, 0), ==, -1);
  munit_assert_float(cube_map_x(1, 0, 1), ==,  1);
  munit_assert_float(cube_map_y(1, 0, 0), ==,  1);
  munit_assert_float(cube_map_y(1, 1, 0), ==, -1);
  return MUNIT_OK;
}

static MunitResult test_cube_face_2(const MunitParameter params[], void *data) {
  munit_assert_float(cube_map_x(2, 0, 0), ==,  1);
  munit_assert_float(cube_map_y(2, 0, 0), ==,  1);
  munit_assert_float(cube_map_y(2, 1, 0), ==, -1);
  munit_assert_float(cube_map_z(2, 0, 0), ==,  1);
  munit_assert_float(cube_map_z(2, 0, 1), ==, -1);
  return MUNIT_OK;
}

static MunitResult test_cube_face_3(const MunitParameter params[], void *data) {
  munit_assert_float(cube_map_z(3, 0, 0), ==, -1);
  munit_assert_float(cube_map_x(3, 0, 0), ==,  1);
  munit_assert_float(cube_map_x(3, 0, 1), ==, -1);
  munit_assert_float(cube_map_y(3, 0, 0), ==,  1);
  munit_assert_float(cube_map_y(3, 1, 0), ==, -1);
  return MUNIT_OK;
}

static MunitResult test_cube_face_4(const MunitParameter params[], void *data) {
  munit_assert_float(cube_map_x(4, 0, 0), ==, -1);
  munit_assert_float(cube_map_y(4, 0, 0), ==,  1);
  munit_assert_float(cube_map_y(4, 1, 0), ==, -1);
  munit_assert_float(cube_map_z(4, 0, 0), ==, -1);
  munit_assert_float(cube_map_z(4, 0, 1), ==,  1);
  return MUNIT_OK;
}

static MunitResult test_cube_face_5(const MunitParameter params[], void *data) {
  munit_assert_float(cube_map_y(5, 0, 0), ==, -1);
  munit_assert_float(cube_map_x(5, 0, 0), ==, -1);
  munit_assert_float(cube_map_x(5, 0, 1), ==,  1);
  munit_assert_float(cube_map_z(5, 0, 0), ==,  1);
  munit_assert_float(cube_map_z(5, 1, 0), ==, -1);
  return MUNIT_OK;
}

static MunitResult test_longitude(const MunitParameter params[], void *data) {
  munit_assert_double_equal(longitude(0, 0, 1), 0, 6);
  munit_assert_double_equal(longitude(1, 0, 0), M_PI / 2, 6);
  return MUNIT_OK;
}

static MunitResult test_lattitude(const MunitParameter params[], void *data) {
  munit_assert_double_equal(lattitude(0, 0, 1), 0, 6);
  munit_assert_double_equal(lattitude(0, 1, 0), M_PI / 2, 6);
  munit_assert_double_equal(lattitude(0.5, 0.5, 0), M_PI / 4, 6);
  return MUNIT_OK;
}

static MunitResult test_map_x(const MunitParameter params[], void *data) {
  munit_assert_int(map_x(0, 675, 3), ==, 675 * 2 * 8);
  munit_assert_int(map_x(-M_PI, 675, 3), ==, 0);
  munit_assert_int(map_x(+M_PI, 675, 3), ==, 0);
  return MUNIT_OK;
}

static MunitResult test_map_y(const MunitParameter params[], void *data) {
  munit_assert_int(map_y(0, 675, 3), ==, 675 * 8);
  munit_assert_int(map_y(M_PI / 2, 675, 3), ==, 0);
  munit_assert_int(map_y(-M_PI / 2, 675, 3), ==, 675 * 8 * 2 - 1);
  return MUNIT_OK;
}

MunitTest test_map[] = {
  {"/cube_face_0", test_cube_face_0, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_face_1", test_cube_face_1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_face_2", test_cube_face_2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_face_3", test_cube_face_3, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_face_4", test_cube_face_4, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_face_5", test_cube_face_5, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/longitude"  , test_longitude  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/lattitude"  , test_lattitude  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/map_x"      , test_map_x      , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/map_y"      , test_map_y      , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL          , NULL            , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
