#include <tgmath.h>
#include "sfsim2025/map.h"
#include "test_map.h"
#include "test_helper.h"


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
  munit_assert_double_equal(longitude(1, 0, 0), 0, 6);
  munit_assert_double_equal(longitude(0, 0, 1), M_PI / 2, 6);
  return MUNIT_OK;
}

static MunitResult test_latitude(const MunitParameter params[], void *data) {
  munit_assert_double_equal(latitude(0, 0, 1), 0, 6);
  munit_assert_double_equal(latitude(0, 1, 0), M_PI / 2, 6);
  munit_assert_double_equal(latitude(0.5, 0.5, 0), M_PI / 4, 6);
  return MUNIT_OK;
}

static MunitResult test_map_x(const MunitParameter params[], void *data) {
  munit_assert_double_equal(map_x(M_PI, 675, 3), 0, 3);
  munit_assert_double_equal(map_x(0, 675, 3), 675 * 2 * 8, 3);
  return MUNIT_OK;
}

static MunitResult test_map_y(const MunitParameter params[], void *data) {
  munit_assert_double_equal(map_y(M_PI / 2, 675, 3), 0, 3);
  munit_assert_double_equal(map_y(0, 675, 3), 675 * 8, 3);
  return MUNIT_OK;
}

static MunitResult test_map_pixels_x(const MunitParameter params[], void *data) {
  int x0, x1;
  float frac0, frac1;
  map_pixels_x(0, 675, 3, &x0, &x1, &frac0, &frac1);
  munit_assert_int(x0, ==, 675 * 2 * 8);
  munit_assert_int(x1, ==, 675 * 2 * 8 + 1);
  munit_assert_float(frac0, ==, 1);
  munit_assert_float(frac1, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_wrap_x0(const MunitParameter params[], void *data) {
  int x0, x1;
  float frac0, frac1;
  map_pixels_x(-M_PI, 675, 3, &x0, &x1, &frac0, &frac1);
  munit_assert_int(x0, ==, 0);
  munit_assert_int(x1, ==, 1);
  munit_assert_float(frac0, ==, 1);
  munit_assert_float(frac1, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_wrap_x1(const MunitParameter params[], void *data) {
  int x0, x1;
  float frac0, frac1;
  map_pixels_x(-M_PI + M_PI / (256 * 4), 256, 0, &x0, &x1, &frac0, &frac1);
  munit_assert_int(x0, ==, 256 * 4 - 1);
  munit_assert_int(x1, ==, 0);
  munit_assert_double_equal(frac0, 0.5, 6);
  munit_assert_double_equal(frac1, 0.5, 6);
  return MUNIT_OK;
}

static MunitResult test_map_pixels_y(const MunitParameter params[], void *data) {
  int y0, y1;
  float frac0, frac1;
  map_pixels_y(0, 675, 3, &y0, &y1, &frac0, &frac1);
  munit_assert_int(y0, ==, 675 * 8);
  munit_assert_int(y1, ==, 675 * 8 + 1);
  munit_assert_float(frac0, ==, 1);
  munit_assert_float(frac1, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_clip_y0(const MunitParameter params[], void *data) {
  int y0, y1;
  float frac0, frac1;
  map_pixels_y(-M_PI / 2, 675, 3, &y0, &y1, &frac0, &frac1);
  munit_assert_int(y0, ==, 675 * 8 * 2 - 1);
  munit_assert_int(y1, ==, 675 * 8 * 2 - 1);
  munit_assert_float(frac0, ==, 1);
  munit_assert_float(frac1, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_frac_y0(const MunitParameter params[], void *data) {
  int y0, y1;
  float frac0, frac1;
  map_pixels_y(M_PI / (4 * 256), 256, 0, &y0, &y1, &frac0, &frac1);
  munit_assert_int(y0, ==, 256 - 1);
  munit_assert_int(y1, ==, 256);
  munit_assert_float(frac0, ==, 0.5);
  munit_assert_float(frac1, ==, 0.5);
  return MUNIT_OK;
}

static MunitResult test_cube_coordinate(const MunitParameter params[], void *data) {
  munit_assert_float(cube_coordinate(0, 256, 0, 0), ==, 0);
  munit_assert_float(cube_coordinate(0, 256, 0, 255), ==, 1.0);
  munit_assert_float(cube_coordinate(1, 256, 0, 255), ==, 0.5);
  munit_assert_float(cube_coordinate(1, 256, 1, 255), ==, 1.0);
  return MUNIT_OK;
}
static MunitResult test_cube_indices(const MunitParameter params[], void *data) {
  int *indices = cube_indices(4);
  munit_assert_int(indices[0], ==, 1);
  munit_assert_int(indices[1], ==, 0);
  munit_assert_int(indices[2], ==, 4);
  munit_assert_int(indices[3], ==, 4);
  munit_assert_int(indices[4], ==, 5);
  munit_assert_int(indices[5], ==, 1);
  return MUNIT_OK;
}

static MunitResult test_scale_point(const MunitParameter params[], void *data) {
  float x, y, z;
  scale_point(0, 2, 0, 100, 80, &x, &y, &z);
  munit_assert_float(x, ==, 0);
  munit_assert_float(y, ==, 80);
  munit_assert_float(z, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_spherical_map(const MunitParameter params[], void *data) {
  float x, y, z;
  spherical_map(0, 0.5, 0.5, 6378000, 6357000, &x, &y, &z);
  munit_assert_float(x, ==, 0);
  munit_assert_float(y, ==, 6357000);
  munit_assert_float(z, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_tile_center(const MunitParameter params[], void *data) {
  float x, y, z;
  tile_center(0, 5, 0, 0, 6378000, 6357000, &x, &y, &z);
  munit_assert_float(x, ==, 0);
  munit_assert_float(y, ==, -6357000);
  munit_assert_float(z, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_offset_longitude(const MunitParameter parms[], void *data) {
  float dx, dy, dz;
  offset_longitude(1, 0, 0, 0, 675, &dx, &dy, &dz);
  munit_assert_double_equal(dx, 0, 6);
  munit_assert_double_equal(dy, 0, 6);
  munit_assert_double_equal(dz, -2 * M_PI / (4 * 675), 6);
  return MUNIT_OK;
}

static MunitResult test_rotate_offset(const MunitParameter params[], void *data) {
  float dx, dy, dz;
  offset_longitude(0, 0, 1, 0, 675, &dx, &dy, &dz);
  munit_assert_double_equal(dx, 2 * M_PI / (4 * 675), 6);
  munit_assert_double_equal(dy, 0, 6);
  munit_assert_double_equal(dz, 0, 6);
  return MUNIT_OK;
}

static MunitResult test_scale_offset(const MunitParameter params[], void *data) {
  float dx, dy, dz;
  offset_longitude(0, 0, 2, 0, 675, &dx, &dy, &dz);
  munit_assert_double_equal(dx, 4 * M_PI / (4 * 675), 6);
  munit_assert_double_equal(dy, 0, 6);
  munit_assert_double_equal(dz, 0, 6);
  return MUNIT_OK;
}

static MunitResult test_level_offset(const MunitParameter params[], void *data) {
  float dx, dy, dz;
  offset_longitude(0, 0, 2, 1, 675, &dx, &dy, &dz);
  munit_assert_double_equal(dx, 2 * M_PI / (4 * 675), 6);
  munit_assert_double_equal(dy, 0, 6);
  munit_assert_double_equal(dz, 0, 6);
  return MUNIT_OK;
}

static MunitResult test_offset_latitude(const MunitParameter params[], void *data) {
  float dx, dy, dz;
  offset_latitude(1, 0, 0, 0, 675, &dx, &dy, &dz);
  munit_assert_double_equal(dx, 0, 6);
  munit_assert_double_equal(dy, 2 * M_PI / (4 * 675), 6);
  munit_assert_double_equal(dz, 0, 6);
  return MUNIT_OK;
}

static MunitResult test_rotate_offset2(const MunitParameter params[], void *data) {
  float dx, dy, dz;
  offset_latitude(0, 1, 0, 0, 675, &dx, &dy, &dz);
  munit_assert_double_equal(dx, -2 * M_PI / (4 * 675), 6);
  munit_assert_double_equal(dy, 0, 6);
  munit_assert_double_equal(dz, 0, 6);
  return MUNIT_OK;
}

static MunitResult test_scale_offset2(const MunitParameter params[], void *data) {
  float dx, dy, dz;
  offset_latitude(2, 0, 0, 0, 675, &dx, &dy, &dz);
  munit_assert_double_equal(dx, 0, 6);
  munit_assert_double_equal(dy, 4 * M_PI / (4 * 675), 6);
  munit_assert_double_equal(dz, 0, 6);
  return MUNIT_OK;
}

static MunitResult test_level_offset2(const MunitParameter params[], void *data) {
  float dx, dy, dz;
  offset_latitude(2, 0, 0, 1, 675, &dx, &dy, &dz);
  munit_assert_double_equal(dx, 0, 6);
  munit_assert_double_equal(dy, 2 * M_PI / (4 * 675), 6);
  munit_assert_double_equal(dz, 0, 6);
  return MUNIT_OK;
}

static MunitResult test_rotate_offset3(const MunitParameter params[], void *data) {
  float dx, dy, dz;
  offset_latitude(0, 1, 1e-8, 0, 675, &dx, &dy, &dz);
  munit_assert_double_equal(dx, 0, 6);
  munit_assert_double_equal(dy, 0, 6);
  munit_assert_double_equal(dz, -2 * M_PI / (4 * 675), 6);
  return MUNIT_OK;
}

MunitTest test_map[] = {
  {"/cube_face_0"     , test_cube_face_0     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_face_1"     , test_cube_face_1     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_face_2"     , test_cube_face_2     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_face_3"     , test_cube_face_3     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_face_4"     , test_cube_face_4     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_face_5"     , test_cube_face_5     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/longitude"       , test_longitude       , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/latitude"        , test_latitude        , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/map_x"           , test_map_x           , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/map_y"           , test_map_y           , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/map_pixels_x"    , test_map_pixels_x    , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/wrap_x0"         , test_wrap_x0         , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/wrap_x1"         , test_wrap_x1         , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/map_pixels_y"    , test_map_pixels_y    , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/clip_y0"         , test_clip_y0         , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/frac_y0"         , test_frac_y0         , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_coordinate" , test_cube_coordinate , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/cube_indices"    , test_cube_indices    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_point"     , test_scale_point     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/spherical_map"   , test_spherical_map   , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/tile_center"     , test_tile_center     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/offset_longitude", test_offset_longitude, NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotate_offset"   , test_rotate_offset   , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_offset"    , test_scale_offset    , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/level_offset"    , test_level_offset    , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/offset_latitude" , test_offset_latitude , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotate_offset2"  , test_rotate_offset2  , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_offset2"   , test_scale_offset2   , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/level_offset2"   , test_level_offset2   , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotate_offset3"  , test_rotate_offset3  , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {NULL               , NULL                 , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
