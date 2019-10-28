#include "sfsim2025/joint.h"
#include "test_joint.h"
#include "test_helper.h"


static MunitResult test_mass_matrix(const MunitParameter params[], void *data) {
  body_info_t info1 = body_info(2, diagonal(3, 5, 7), vector(0, 0, 0), vector(0, 0, 0));
  body_info_t info2 = body_info(3, diagonal(5, 7, 9), vector(0, 0, 0), vector(0, 0, 0));
  state_t *s = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  large_matrix_t m = joint_mass(info1, info2, s, s);
  munit_assert_int(m.rows, ==, 12);
  munit_assert_int(m.cols, ==, 12);
  munit_assert_double(m.data[  0], ==, 2); munit_assert_double(m.data[ 13], ==, 2); munit_assert_double(m.data[ 26], ==, 2);
  munit_assert_double(m.data[ 39], ==, 3); munit_assert_double(m.data[ 52], ==, 5); munit_assert_double(m.data[ 65], ==, 7);
  munit_assert_double(m.data[ 78], ==, 3); munit_assert_double(m.data[ 91], ==, 3); munit_assert_double(m.data[104], ==, 3);
  munit_assert_double(m.data[117], ==, 5); munit_assert_double(m.data[130], ==, 7); munit_assert_double(m.data[143], ==, 9);
  return MUNIT_OK;
}

static MunitResult test_rotate_inertia(const MunitParameter params[], void *data) {
  body_info_t info1 = body_info(2, diagonal(3, 5, 7), vector(0, 0, 0), vector(0, 0, 0));
  body_info_t info2 = body_info(3, diagonal(5, 7, 9), vector(0, 0, 0), vector(0, 0, 0));
  state_t *s = state(vector(0, 0, 0), vector(0, 0, 0), quaternion_rotation(M_PI / 2, vector(1, 0, 0)), vector(0, 0, 0));
  large_matrix_t m = joint_mass(info1, info2, s, s);
  munit_assert_double(m.data[ 39], ==, 3); munit_assert_double(m.data[ 52], ==, 7); munit_assert_double(m.data[ 65], ==, 5);
  return MUNIT_OK;
}

MunitTest test_joint[] = {
  {"/mass_matrix"   , test_mass_matrix   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotate_inertia", test_rotate_inertia, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL             , NULL               , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
