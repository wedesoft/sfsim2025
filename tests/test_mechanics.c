#include <gc.h>
#include "sfsim2025/mechanics.h"
#include "test_helper.h"
#include "test_mechanics.h"


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

static MunitResult test_inertia_unit_cube(const MunitParameter params[], void *data) {
  matrix_t result = inertia_cuboid(6, 1, 1, 1);
  munit_assert_double(result.m11, ==, 1); munit_assert_double(result.m12, ==, 0); munit_assert_double(result.m13, ==, 0);
  munit_assert_double(result.m21, ==, 0); munit_assert_double(result.m22, ==, 1); munit_assert_double(result.m23, ==, 0);
  munit_assert_double(result.m31, ==, 0); munit_assert_double(result.m32, ==, 0); munit_assert_double(result.m33, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_heavy_cube(const MunitParameter params[], void *data) {
  matrix_t result = inertia_cuboid(12, 1, 1, 1);
  munit_assert_double(result.m11, ==, 2); munit_assert_double(result.m12, ==, 0); munit_assert_double(result.m13, ==, 0);
  munit_assert_double(result.m21, ==, 0); munit_assert_double(result.m22, ==, 2); munit_assert_double(result.m23, ==, 0);
  munit_assert_double(result.m31, ==, 0); munit_assert_double(result.m32, ==, 0); munit_assert_double(result.m33, ==, 2);
  return MUNIT_OK;
}

static MunitResult test_inertia_cuboid(const MunitParameter params[], void *data) {
  matrix_t result = inertia_cuboid(12, 2, 3, 5);
  munit_assert_double(result.m11, ==, 34); munit_assert_double(result.m12, ==,  0); munit_assert_double(result.m13, ==,  0);
  munit_assert_double(result.m21, ==,  0); munit_assert_double(result.m22, ==, 29); munit_assert_double(result.m23, ==,  0);
  munit_assert_double(result.m31, ==,  0); munit_assert_double(result.m32, ==,  0); munit_assert_double(result.m33, ==, 13);
  return MUNIT_OK;
}

static MunitResult test_inertia_sphere(const MunitParameter params[], void *data) {
  matrix_t result = inertia_sphere(5, 3);
  munit_assert_double(result.m11, ==, 18); munit_assert_double(result.m12, ==,  0); munit_assert_double(result.m13, ==,  0);
  munit_assert_double(result.m21, ==,  0); munit_assert_double(result.m22, ==, 18); munit_assert_double(result.m23, ==,  0);
  munit_assert_double(result.m31, ==,  0); munit_assert_double(result.m32, ==,  0); munit_assert_double(result.m33, ==, 18);
  return MUNIT_OK;
}

static MunitResult test_predict_speed(const MunitParameter params[], void *data) {
  body_t b = body(0.25, diagonal(1, 1, 1));
  state_t *s = state(vector(1, 2, 3), vector(3, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  forces_t f = forces(vector(1, 0, 0), vector(0, 0, 0));
  state_t *result = predict(s, b, f, vector(0, 0, 0), vector(0, 0, 0), 0.5);
  munit_assert_double(result->position.x, ==, 1);
  munit_assert_double(result->position.y, ==, 2);
  munit_assert_double(result->position.z, ==, 3);
  munit_assert_double(result->speed.x, ==, 5);
  munit_assert_double(result->speed.y, ==, 0);
  munit_assert_double(result->speed.z, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_consider_linear_impulse(const MunitParameter params[], void *data) {
  body_t b = body(0.25, diagonal(1, 1, 1));
  state_t *s = state(vector(1, 2, 3), vector(3, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  forces_t f = forces(vector(0, 0, 0), vector(0, 0, 0));
  state_t *result = predict(s, b, f, vector(1, 0, 0), vector(0, 0, 0), 0.5);
  munit_assert_double(result->speed.x, ==, 7);
  munit_assert_double(result->speed.y, ==, 0);
  munit_assert_double(result->speed.z, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_predict_rotation(const MunitParameter params[], void *data) {
  body_t b = body(1, diagonal(1, 1, 1));
  state_t *s = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(2, 0, 0));
  forces_t f = forces(vector(0, 0, 0), vector(3, 0, 0));
  state_t *result = predict(s, b, f, vector(0, 0, 0), vector(0, 0, 0), 0.5);
  munit_assert_double(result->orientation.a, ==, 1);
  munit_assert_double(result->orientation.b, ==, 0);
  munit_assert_double(result->orientation.c, ==, 0);
  munit_assert_double(result->orientation.d, ==, 0);
  munit_assert_double(result->rotation.x, ==, 3.5);
  munit_assert_double(result->rotation.y, ==, 0);
  munit_assert_double(result->rotation.z, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_consider_angular_impulse(const MunitParameter params[], void *data) {
  body_t b = body(1, diagonal(1, 1, 1));
  state_t *s = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(2, 0, 0));
  forces_t f = forces(vector(0, 0, 0), vector(0, 0, 0));
  state_t *result = predict(s, b, f, vector(0, 0, 0), vector(3, 0, 0), 0.5);
  munit_assert_double(result->rotation.x, ==, 5);
  munit_assert_double(result->rotation.y, ==, 0);
  munit_assert_double(result->rotation.z, ==, 0);
  return MUNIT_OK;
}

MunitTest test_mechanics[] = {
  {"/runge_kutta"             , test_runge_kutta             , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/inertia_unit_cube"       , test_inertia_unit_cube       , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/heavy_cube"              , test_heavy_cube              , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/inertia_cuboid"          , test_inertia_cuboid          , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/inertia_sphere"          , test_inertia_sphere          , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/predict_speed"           , test_predict_speed           , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/consider_linear_impulse" , test_consider_linear_impulse , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/predict_rotation"        , test_predict_rotation        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/consider_angular_impulse", test_consider_angular_impulse, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                       , NULL                         , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
