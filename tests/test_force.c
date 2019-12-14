#include "sfsim2025/force.h"
#include "sfsim2025/mechanics.h"
#include "test_force.h"
#include "test_helper.h"


static MunitResult test_gravitation(const MunitParameter params[], void *data) {
  force_t f = gravitation(1, 2);
  munit_assert_int(f.force_type, ==, GRAVITATION);
  munit_assert_int(f.i, ==, 1);
  munit_assert_int(f.j, ==, 2);
  return MUNIT_OK;
}

static MunitResult test_exert_gravitation(const MunitParameter params[], void *data) {
  force_t f = gravitation(1, 2);
  body_t body1 = body(5.9722e+24, inertia_sphere(5.9722e+24, 6.371e+6));
  body_t body2 = body(1, inertia_sphere(1, 1));
  state_t *state1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *state2 = state(vector(0, 6.371e+6, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  vector_t force1; vector_t tau1; vector_t force2; vector_t tau2;
  exert_force(body1, body2, f, state1, state2, &force1, &force2, &tau1, &tau2);
  munit_assert_double_equal(force1.y, +9.82, 3);
  munit_assert_double_equal(force2.y, -9.82, 3);
  return MUNIT_OK;
}

MunitTest test_force[] = {
  {"/gravitation"      , test_gravitation      , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/exert_gravitation", test_exert_gravitation, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                , NULL                  , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
