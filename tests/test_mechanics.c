#include <gc.h>
#include "sfsim2025/mechanics.h"
#include "test_helper.h"
#include "test_mechanics.h"


static MunitResult test_state(const MunitParameter params[], void *data) {
  state_t *result = state(vector(2, 3, 5), vector(3, 5, 7), quaternion(1, 0, 0, 0), vector(5, 7, 11));
  munit_assert_double(result->position.x, ==, 2);
  munit_assert_double(result->linear_momentum.x, ==, 3);
  munit_assert_double(result->orientation.a, ==, 1);
  munit_assert_double(result->angular_momentum.x, ==, 5);
  return MUNIT_OK;
}

static void *f(double t, double dt, void *y_, void *data) {
  double *y = y_;
  double *acceleration = data;
  double *result = GC_MALLOC_ATOMIC(sizeof(double[2]));
  result[0] = y[1] * dt;
  result[1] = *acceleration * dt;
  return result;
}

static void *add(void *y_, void *dy_) {
  double *y = y_;
  double *dy = dy_;
  double *result = GC_MALLOC_ATOMIC(sizeof(double[2]));
  result[0] = y[0] + dy[0];
  result[1] = y[1] + dy[1];
  return result;
}

static void *scale(void *y_, double s) {
  double *y = y_;
  double *result = GC_MALLOC_ATOMIC(sizeof(double[2]));
  result[0] = y[0] * s;
  result[1] = y[1] * s;
  return result;
}

static MunitResult test_runge_kutta(const MunitParameter params[], void *data) {
  double initial[2] = {0.0, 0.0};
  double acceleration = 3.0;
  double *final = runge_kutta(initial, 4, f, add, scale, &acceleration);
  munit_assert_double_equal(final[1], 12, 6);
  munit_assert_double_equal(final[0], 24, 6);
  return MUNIT_OK;
}

static MunitResult test_position_change(const MunitParameter params[], void *data) {
  state_t *s = state(vector(0, 0, 0), vector(1, 2, 3), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  body_info_t info = { .mass = 2.0, .force = vector(0, 0, 0), .torque = vector(0, 0, 0) };
  state_t *ds = state_change(0, 4, s, &info);
  munit_assert_double(ds->position.x, ==, 2);
  munit_assert_double(ds->position.y, ==, 4);
  munit_assert_double(ds->position.z, ==, 6);
  return MUNIT_OK;
}

static MunitResult test_momentum_change(const MunitParameter params[], void *data) {
  state_t *s = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  body_info_t info = { .mass = 1.0, .force = vector(1, 2, 3), .torque = vector(0, 0, 0) };
  state_t *ds = state_change(0, 2, s, &info);
  munit_assert_double(ds->linear_momentum.x, ==, 2);
  munit_assert_double(ds->linear_momentum.y, ==, 4);
  munit_assert_double(ds->linear_momentum.z, ==, 6);
  return MUNIT_OK;
}

static MunitResult test_angular_momentum(const MunitParameter params[], void *data) {
  state_t *s = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  body_info_t info = { .mass = 1.0, .force = vector(1, 2, 3), .torque = vector(1, 2, 3) };
  state_t *ds = state_change(0, 2, s, &info);
  munit_assert_double(ds->angular_momentum.x, ==, 2);
  munit_assert_double(ds->angular_momentum.y, ==, 4);
  munit_assert_double(ds->angular_momentum.z, ==, 6);
  return MUNIT_OK;
}

static MunitResult test_zero_angular_speed(const MunitParameter params[], void *data) {
  vector_t result = angular_velocity(diagonal(1, 2, 4), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  munit_assert_double(result.x, ==, 0); munit_assert_double(result.y, ==, 0); munit_assert_double(result.z, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_rotational_inertia(const MunitParameter params[], void *data) {
  vector_t result = angular_velocity(diagonal(1, 2, 4), quaternion(1, 0, 0, 0), vector(2, 4, 6));
  munit_assert_double_equal(result.x, 2, 6); munit_assert_double_equal(result.y, 2, 6); munit_assert_double_equal(result.z, 1.5, 6);
  return MUNIT_OK;
}

static MunitResult test_rotated_inertia(const MunitParameter params[], void *data) {
  vector_t result = angular_velocity(diagonal(1, 2, 4), quaternion_rotation(M_PI / 2, vector(1, 0, 0)), vector(2, 4, 6));
  munit_assert_double_equal(result.x, 2, 6); munit_assert_double_equal(result.y, 1, 6); munit_assert_double_equal(result.z, 3, 6);
  return MUNIT_OK;
}

static MunitResult test_orientation_change(const MunitParameter params[], void *data) {
  return MUNIT_SKIP;
}

MunitTest test_mechanics[] = {
  {"/state"             , test_state             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/runge_kutta"       , test_runge_kutta       , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/position_change"   , test_position_change   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/momentum_change"   , test_momentum_change   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/angular_momentum"  , test_angular_momentum  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/zero_angular_speed", test_zero_angular_speed, NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotational_inertia", test_rotational_inertia, NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotated_inertia"   , test_rotated_inertia   , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/orientation_change", test_orientation_change, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                 , NULL                   , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
