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

static MunitResult test_speed_vector(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(2, 3,  5), quaternion(1, 0, 0, 0), vector(3, 5,  7));
  state_t *s2 = state(vector(0, 0, 0), vector(5, 7, 11), quaternion(1, 0, 0, 0), vector(7, 11, 13));
  large_vector_t u = speed_vector(s1, s2);
  munit_assert_int(u.rows, ==, 12);
  munit_assert_double(u.data[0], ==, 2); munit_assert_double(u.data[ 1], ==,  3); munit_assert_double(u.data[ 2], ==,  5);
  munit_assert_double(u.data[3], ==, 3); munit_assert_double(u.data[ 4], ==,  5); munit_assert_double(u.data[ 5], ==,  7);
  munit_assert_double(u.data[6], ==, 5); munit_assert_double(u.data[ 7], ==,  7); munit_assert_double(u.data[ 8], ==, 11);
  munit_assert_double(u.data[9], ==, 7); munit_assert_double(u.data[10], ==, 11); munit_assert_double(u.data[11], ==, 13);
  return MUNIT_OK;
}

static MunitResult test_ball_jacobian(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 0, 4), vector(0, 0, 0), quaternion_rotation(M_PI / 2, vector(1, 0, 0)), vector(0, 0, 0));
  large_matrix_t j = ball_in_socket_jacobian(s1, s2, joint(vector(0, 0, 2), vector(0, 0, -2)));
  munit_assert_int(j.rows, ==, 3);
  munit_assert_int(j.cols, ==, 12);
  munit_assert_double(j.data[0], ==,  1); munit_assert_double(j.data[13], ==,  1); munit_assert_double(j.data[26], ==,  1);
  munit_assert_double(j.data[6], ==, -1); munit_assert_double(j.data[19], ==, -1); munit_assert_double(j.data[32], ==, -1);
  munit_assert_double_equal(j.data[ 4],  2, 6);
  munit_assert_double_equal(j.data[15], -2, 6);
  munit_assert_double_equal(j.data[11],  2, 6);
  munit_assert_double_equal(j.data[33], -2, 6);
  return MUNIT_OK;
}

MunitTest test_joint[] = {
  {"/mass_matrix"   , test_mass_matrix   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotate_inertia", test_rotate_inertia, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/speed_vector"  , test_speed_vector  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/ball_jacobian" , test_ball_jacobian , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL             , NULL               , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
