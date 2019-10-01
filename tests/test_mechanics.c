#include <gc.h>
#include "sfsim2025/mechanics.h"
#include "test_helper.h"
#include "test_mechanics.h"


static MunitResult test_state(const MunitParameter params[], void *data) {
  state_t result = state(vector(2, 3, 5), vector(3, 5, 7), quaternion(1, 0, 0, 0), vector(5, 7, 11));
  munit_assert_double(result.position.x, ==, 2);
  munit_assert_double(result.linear_momentum.x, ==, 3);
  munit_assert_double(result.orientation.a, ==, 1);
  munit_assert_double(result.angular_momentum.x, ==, 5);
  return MUNIT_OK;
}

static void *f(double t, double dt, void *y_, void *data) {
  double *acceleration = data;
  double *y = y_;
  double *result = GC_MALLOC_ATOMIC(2 * sizeof(double));
  result[0] = y[1] * dt;
  result[1] = *acceleration * dt;
  return result;
}

static void *add(void *y_, void *dy_) {
  double *y = y_;
  double *dy = dy_;
  double *result = GC_MALLOC_ATOMIC(2 * sizeof(double));
  result[0] = y[0] + dy[0];
  result[1] = y[1] + dy[1];
  return result;
}

static void *scale(void *y_, double s) {
  double *y = y_;
  double *result = GC_MALLOC_ATOMIC(2 * sizeof(double));
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

MunitTest test_mechanics[] = {
  {"/state"        , test_state      , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/runge_kutta"  , test_runge_kutta, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL            , NULL            , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
