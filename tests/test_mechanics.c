#include <gc.h>
#include "sfsim2025/mechanics.h"
#include "test_helper.h"
#include "test_mechanics.h"


static void *f(double t, void *y_) {
  double *y = y_;
  double *result = GC_MALLOC_ATOMIC(2 * sizeof(double));
  result[0] = y[1];
  result[1] = 1.0;
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
  double *final = runge_kutta(0, initial, 2, f, add, scale);
  munit_assert_double_equal(final[1], 2, 6);
  munit_assert_double_equal(final[0], 2, 6);
  return MUNIT_OK;
}

MunitTest test_mechanics[] = {
  {"/runge_kutta"  , test_runge_kutta, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL            , NULL            , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
