#include "sfsim2025/joint.h"
#include "sfsim2025/mechanics.h"
#include "test_joint.h"
#include "test_helper.h"


static MunitResult test_mass_matrix(const MunitParameter params[], void *data) {
  body_t body1 = body(2, diagonal(3, 5, 7));
  body_t body2 = body(3, diagonal(5, 7, 9));
  state_t *s = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  large_matrix_t m = joint_mass(body1, body2, s, s);
  munit_assert_int(m.rows, ==, 12);
  munit_assert_int(m.cols, ==, 12);
  munit_assert_double(m.data[  0], ==, 2); munit_assert_double(m.data[ 13], ==, 2); munit_assert_double(m.data[ 26], ==, 2);
  munit_assert_double(m.data[ 39], ==, 3); munit_assert_double(m.data[ 52], ==, 5); munit_assert_double(m.data[ 65], ==, 7);
  munit_assert_double(m.data[ 78], ==, 3); munit_assert_double(m.data[ 91], ==, 3); munit_assert_double(m.data[104], ==, 3);
  munit_assert_double(m.data[117], ==, 5); munit_assert_double(m.data[130], ==, 7); munit_assert_double(m.data[143], ==, 9);
  return MUNIT_OK;
}

static MunitResult test_rotate_inertia(const MunitParameter params[], void *data) {
  body_t body1 = body(2, diagonal(3, 5, 7));
  body_t body2 = body(3, diagonal(5, 7, 9));
  state_t *s = state(vector(0, 0, 0), vector(0, 0, 0), quaternion_rotation(M_PI / 2, vector(1, 0, 0)), vector(0, 0, 0));
  large_matrix_t m = joint_mass(body1, body2, s, s);
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
  large_matrix_t j = ball_in_socket_jacobian(s1, s2, ball_in_socket(0, 1, vector(0, 0, 2), vector(0, 0, -2)).ball_in_socket);
  munit_assert_int(j.rows, ==,  3);
  munit_assert_int(j.cols, ==, 12);
  munit_assert_double(j.data[0], ==,  1); munit_assert_double(j.data[13], ==,  1); munit_assert_double(j.data[26], ==,  1);
  munit_assert_double(j.data[6], ==, -1); munit_assert_double(j.data[19], ==, -1); munit_assert_double(j.data[32], ==, -1);
  munit_assert_double_equal(j.data[ 4],  2, 6);
  munit_assert_double_equal(j.data[15], -2, 6);
  munit_assert_double_equal(j.data[11],  2, 6);
  munit_assert_double_equal(j.data[33], -2, 6);
  return MUNIT_OK;
}

static MunitResult test_ball_correction(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(-4, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector( 5, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  large_vector_t b = ball_in_socket_correction(s1, s2, ball_in_socket(0, 1, vector(2, 0, 0), vector(-3, 0, 0)).ball_in_socket);
  munit_assert_int(b.rows, ==, 3);
  munit_assert_double_equal(b.data[0], -4, 6);
  munit_assert_double_equal(b.data[1],  0, 6);
  munit_assert_double_equal(b.data[2],  0, 6);
  return MUNIT_OK;
}

static MunitResult test_rotate_correction(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(-4, 0, 0), vector(0, 0, 0), quaternion_rotation(M_PI, vector(0, 0, 1)), vector(0, 0, 0));
  state_t *s2 = state(vector( 5, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  large_vector_t b = ball_in_socket_correction(s1, s2, ball_in_socket(0, 0, vector(2, 0, 0), vector(-3, 0, 0)).ball_in_socket);
  munit_assert_int(b.rows, ==, 3);
  munit_assert_double_equal(b.data[0], -8, 6);
  munit_assert_double_equal(b.data[1],  0, 6);
  munit_assert_double_equal(b.data[2],  0, 6);
  return MUNIT_OK;
}

static MunitResult test_correcting_impulse(const MunitParameter params[], void *data) {
  body_t body1 = body(5.9722e+24, inertia_sphere(5.9722e+24, 6370000));
  body_t body2 = body(1.0, inertia_cuboid(1.0, 0.1, 2, 0.1));
  state_t *s1 = state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 1, 0), vector(0, -0.01, 0), quaternion_rotation(0, vector(0, 0, 1)), vector(0, 0, 0));
  joint_t jnt = ball_in_socket(0, 1, vector(0, 6370002, 0), vector(0, 1, 0));
  large_matrix_t j = ball_in_socket_jacobian(s1, s2, jnt.ball_in_socket);
  large_vector_t b = ball_in_socket_correction(s1, s2, jnt.ball_in_socket);
  vector_t p1; vector_t t1; vector_t p2; vector_t t2;
  correcting_impulse(body1, body2, s1, s2, j, b, &p1, &p2, &t1, &t2);
  munit_assert_double_equal(p1.x,  0.0 , 6);
  munit_assert_double_equal(p1.y, -0.01, 6);
  munit_assert_double_equal(p1.z,  0.0 , 6);
  munit_assert_double_equal(p2.x,  0.0 , 6);
  munit_assert_double_equal(p2.y,  0.01, 6);
  munit_assert_double_equal(p2.z,  0.0 , 6);
  return MUNIT_OK;
}

static MunitResult test_indices(const MunitParameter params[], void *data) {
  joint_t jnt = ball_in_socket(2, 3, vector(0, 0, 0), vector(0, 0, 0));
  munit_assert_int(jnt.i, ==, 2);
  munit_assert_int(jnt.j, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_joint_impulse(const MunitParameter params[], void *data) {
  body_t body1 = body(5.9722e+24, inertia_sphere(5.9722e+24, 6370000));
  body_t body2 = body(1.0, inertia_cuboid(1.0, 0.1, 2, 0.1));
  state_t *s1 = state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 1, 0), vector(0, -0.01, 0), quaternion_rotation(0, vector(0, 0, 1)), vector(0, 0, 0));
  joint_t jnt = ball_in_socket(0, 1, vector(0, 6370002, 0), vector(0, 1, 0));
  vector_t p1; vector_t t1; vector_t p2; vector_t t2;
  joint_impulse(body1, body2, jnt, s1, s2, &p1, &p2, &t1, &t2);
  munit_assert_double_equal(p1.x,  0.0 , 6);
  munit_assert_double_equal(p1.y, -0.01, 6);
  munit_assert_double_equal(p1.z,  0.0 , 6);
  return MUNIT_OK;
}

static MunitResult test_hinge_jacobian(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 0, 4), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  large_matrix_t j = hinge_jacobian(s1, s2, hinge(0, 1, vector(0, 0, 2), vector(0, 0, -2), vector(1, 0, 0), vector(1, 0, 0)).hinge);
  munit_assert_int(j.rows, ==,  5);
  munit_assert_int(j.cols, ==, 12);
  munit_assert_double(j.data[0], ==,  1); munit_assert_double(j.data[13], ==,  1); munit_assert_double(j.data[26], ==,  1);
  munit_assert_double(j.data[6], ==, -1); munit_assert_double(j.data[19], ==, -1); munit_assert_double(j.data[32], ==, -1);
  munit_assert_double_equal(j.data[ 4],  2, 6);
  munit_assert_double_equal(j.data[15], -2, 6);
  munit_assert_double_equal(j.data[10],  2, 6);
  munit_assert_double_equal(j.data[21], -2, 6);
  munit_assert_double_equal(j.data[40], 1, 6);
  munit_assert_double_equal(j.data[53], 1, 6);
  munit_assert_double_equal(j.data[46], -1, 6);
  munit_assert_double_equal(j.data[59], -1, 6);
  return MUNIT_OK;
}

static MunitResult test_hinge_correction(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(-4, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector( 5, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  large_vector_t b =
    hinge_correction(s1, s2, hinge(0, 1, vector(2, 0, 0), vector(-3, 0, 0), vector(1, 0, 0.1), vector(1, 0, -0.1)).hinge);
  munit_assert_int(b.rows, ==, 5);
  munit_assert_double_equal(b.data[0], -4  , 6);
  munit_assert_double_equal(b.data[1],  0  , 6);
  munit_assert_double_equal(b.data[2],  0  , 6);
  munit_assert_double_equal(b.data[3], -0.2, 6);
  munit_assert_double_equal(b.data[4],  0  , 6);
  return MUNIT_OK;
}

static MunitResult test_slider(const MunitParameter params[], void *data) {
  joint_t joint = slider(2, 3, vector(5, 7, 11), vector(13, 17, 19), quaternion(2, 3, 5, 7), quaternion(11, 13, 17, 19));
  munit_assert_int(joint.joint_type, ==, SLIDER);
  munit_assert_int(joint.i, ==, 2);
  munit_assert_int(joint.j, ==, 3);
  munit_assert_double(joint.slider.r1.x, ==, 5);
  munit_assert_double(joint.slider.r2.x, ==, 13);
  munit_assert_double(joint.slider.q1.a, ==, 2);
  munit_assert_double(joint.slider.q2.a, ==, 11);
  return MUNIT_OK;
}

static MunitResult test_slider_jacobian(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, -2, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(4, 0, -3), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  joint_t joint = slider(0, 1, vector(0, 2, 0), vector(0, 0, 3), quaternion(1, 0, 0, 0), quaternion(1, 0, 0, 0));
  large_matrix_t j = slider_jacobian(s1, s2, joint.slider);
  munit_assert_int(j.rows, ==,  5);
  munit_assert_int(j.cols, ==, 12);
  munit_assert_double(j.data[ 0], ==,  0  ); munit_assert_double(j.data[ 1], ==,  0); munit_assert_double(j.data[ 2], ==,  0);
  munit_assert_double(j.data[12], ==,  0  ); munit_assert_double(j.data[13], ==,  0); munit_assert_double(j.data[14], ==,  0);
  munit_assert_double(j.data[24], ==,  0  ); munit_assert_double(j.data[25], ==,  0); munit_assert_double(j.data[26], ==,  0);
  munit_assert_double(j.data[36], ==,  0  ); munit_assert_double(j.data[37], ==,  1); munit_assert_double(j.data[38], ==,  0);
  munit_assert_double(j.data[48], ==,  0  ); munit_assert_double(j.data[49], ==,  0); munit_assert_double(j.data[50], ==,  1);
  munit_assert_double(j.data[ 6], ==,  0  ); munit_assert_double(j.data[ 7], ==,  0); munit_assert_double(j.data[ 8], ==,  0);
  munit_assert_double(j.data[18], ==,  0  ); munit_assert_double(j.data[19], ==,  0); munit_assert_double(j.data[20], ==,  0);
  munit_assert_double(j.data[30], ==,  0  ); munit_assert_double(j.data[31], ==,  0); munit_assert_double(j.data[32], ==,  0);
  munit_assert_double(j.data[42], ==,  0  ); munit_assert_double(j.data[43], ==, -1); munit_assert_double(j.data[44], ==,  0);
  munit_assert_double(j.data[54], ==,  0  ); munit_assert_double(j.data[55], ==,  0); munit_assert_double(j.data[56], ==, -1);
  munit_assert_double(j.data[ 3], ==,  1  ); munit_assert_double(j.data[ 4], ==,  0); munit_assert_double(j.data[ 5], ==,  0);
  munit_assert_double(j.data[15], ==,  0  ); munit_assert_double(j.data[16], ==,  1); munit_assert_double(j.data[17], ==,  0);
  munit_assert_double(j.data[27], ==,  0  ); munit_assert_double(j.data[28], ==,  0); munit_assert_double(j.data[29], ==,  1);
  munit_assert_double(j.data[39], ==,  1.5); munit_assert_double(j.data[40], ==,  0); munit_assert_double(j.data[41], ==,  2);
  munit_assert_double(j.data[51], ==,  1  ); munit_assert_double(j.data[52], ==, -2); munit_assert_double(j.data[53], ==,  0);
  munit_assert_double(j.data[ 9], ==, -1  ); munit_assert_double(j.data[10], ==,  0); munit_assert_double(j.data[11], ==,  0);
  munit_assert_double(j.data[21], ==,  0  ); munit_assert_double(j.data[22], ==, -1); munit_assert_double(j.data[23], ==,  0);
  munit_assert_double(j.data[33], ==,  0  ); munit_assert_double(j.data[34], ==,  0); munit_assert_double(j.data[35], ==, -1);
  munit_assert_double(j.data[45], ==,  1.5); munit_assert_double(j.data[46], ==,  0); munit_assert_double(j.data[47], ==,  2);
  munit_assert_double(j.data[57], ==,  1  ); munit_assert_double(j.data[58], ==, -2); munit_assert_double(j.data[59], ==,  0);
  return MUNIT_OK;
}

static MunitResult test_slider_correction(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 3, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(4, 0, 5), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  joint_t joint = slider(0, 1, vector(0, -2, 0), vector(0, 0, -3), quaternion(1, 0, 0, 0), quaternion(1, 0, 0, 0));
  large_vector_t b = slider_correction(s1, s2, joint.slider);
  munit_assert_int(b.rows, ==, 5);
  munit_assert_double(b.data[3], ==,  1);
  munit_assert_double(b.data[4], ==, -2);
  return MUNIT_OK;
}

MunitTest test_joint[] = {
  {"/mass_matrix"       , test_mass_matrix       , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotate_inertia"    , test_rotate_inertia    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/speed_vector"      , test_speed_vector      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/ball_jacobian"     , test_ball_jacobian     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/ball_correction"   , test_ball_correction   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotate_correction" , test_rotate_correction , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/correcting_impulse", test_correcting_impulse, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/indices"           , test_indices           , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/joint_impulse"     , test_joint_impulse     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/hinge_jacobian"    , test_hinge_jacobian    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/hinge_correction"  , test_hinge_correction  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/slider"            , test_slider            , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/slider_jacobian"   , test_slider_jacobian   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/slider_correction" , test_slider_correction , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                 , NULL                   , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
