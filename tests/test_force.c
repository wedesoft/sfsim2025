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
  munit_assert_double(tau1.x, ==, 0);
  munit_assert_double(tau2.x, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_fixed_force(const MunitParameter params[], void *data) {
  force_t f = fixed_force(3, vector(1, 2, 3), vector(4, 5, 6));
  munit_assert_int(f.force_type, ==, FIXED_FORCE);
  munit_assert_int(f.i, ==, 3);
  munit_assert_int(f.j, ==, 3);
  munit_assert_double(f.fixed_force.force.x, ==, 1);
  munit_assert_double(f.fixed_force.force.y, ==, 2);
  munit_assert_double(f.fixed_force.force.z, ==, 3);
  munit_assert_double(f.fixed_force.torque.x, ==, 4);
  munit_assert_double(f.fixed_force.torque.y, ==, 5);
  munit_assert_double(f.fixed_force.torque.z, ==, 6);
  return MUNIT_OK;
}

static MunitResult test_exert_fixed_force(const MunitParameter params[], void *data) {
  force_t f = fixed_force(3, vector(1, 2, 3), vector(4, 5, 6));
  body_t b = body(1, inertia_sphere(1, 1));
  state_t *s = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  vector_t force1; vector_t tau1; vector_t force2; vector_t tau2;
  exert_force(b, b, f, s, s, &force1, &force2, &tau1, &tau2);
  munit_assert_double(force1.x, ==, 1);
  munit_assert_double(tau1.x, ==, 4);
  munit_assert_double(force2.x, ==, 0);
  munit_assert_double(tau2.x, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_rotate_fixed_force(const MunitParameter params[], void *data) {
  force_t f = fixed_force(3, vector(1, 2, 3), vector(4, 5, 6));
  body_t b = body(1, inertia_sphere(1, 1));
  state_t *s = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(0, 1, 0, 0), vector(0, 0, 0));
  vector_t force1; vector_t tau1; vector_t force2; vector_t tau2;
  exert_force(b, b, f, s, s, &force1, &force2, &tau1, &tau2);
  munit_assert_double(force1.y, ==, -2);
  munit_assert_double(tau1.y, ==, -5);
  return MUNIT_OK;
}

static MunitResult test_spring_damper(const MunitParameter params[], void *data) {
  force_t f = spring_damper(1, 2, vector(3, 0, 0), vector(4, 0, 0), 5, 6, 7);
  munit_assert_int(f.force_type, ==, SPRING_DAMPER);
  munit_assert_int(f.i, ==, 1);
  munit_assert_int(f.j, ==, 2);
  munit_assert_double(f.spring_damper.r1.x, ==, 3);
  munit_assert_double(f.spring_damper.r2.x, ==, 4);
  munit_assert_double(f.spring_damper.length, ==, 5);
  munit_assert_double(f.spring_damper.k, ==, 6);
  munit_assert_double(f.spring_damper.c, ==, 7);
  return MUNIT_OK;
}

static MunitResult test_spring_neutral(const MunitParameter params[], void *data) {
  force_t f = spring_damper(0, 1, vector(0, 0, 0), vector(0, 0, 0), 5, 10, 2);
  body_t body1 = body(1, inertia_sphere(1, 1));
  body_t body2 = body(1, inertia_sphere(1, 1));
  state_t *state1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *state2 = state(vector(5, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  vector_t force1; vector_t tau1; vector_t force2; vector_t tau2;
  exert_force(body1, body2, f, state1, state2, &force1, &force2, &tau1, &tau2);
  munit_assert_double_equal(force1.x, 0, 6); munit_assert_double_equal(force1.y, 0, 6); munit_assert_double_equal(force1.z, 0, 6);
  munit_assert_double_equal(force2.x, 0, 6); munit_assert_double_equal(force2.y, 0, 6); munit_assert_double_equal(force2.z, 0, 6);
  munit_assert_double_equal(tau1.x, 0, 6); munit_assert_double_equal(tau1.y, 0, 6); munit_assert_double_equal(tau1.z, 0, 6);
  munit_assert_double_equal(tau2.x, 0, 6); munit_assert_double_equal(tau2.y, 0, 6); munit_assert_double_equal(tau2.z, 0, 6);
  return MUNIT_OK;
}
static MunitResult test_spring_tension(const MunitParameter params[], void *data) {
  force_t f = spring_damper(0, 1, vector(0, 0, 0), vector(0, 0, 0), 5, 10, 2);
  body_t body1 = body(1, inertia_sphere(1, 1));
  body_t body2 = body(1, inertia_sphere(1, 1));
  state_t *state1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *state2 = state(vector(6, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  vector_t force1; vector_t tau1; vector_t force2; vector_t tau2;
  exert_force(body1, body2, f, state1, state2, &force1, &force2, &tau1, &tau2);
  munit_assert_double_equal(force1.x,  10, 6);
  munit_assert_double_equal(force2.x, -10, 6);
  return MUNIT_OK;
}

static MunitResult test_spring_torque(const MunitParameter params[], void *data) {
  force_t f = spring_damper(0, 1, vector(0, 1, 0), vector(0, 2, 0), 5, 10, 2);
  body_t body1 = body(1, inertia_sphere(1, 1));
  body_t body2 = body(1, inertia_sphere(1, 1));
  state_t *state1 = state(vector(0,  0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *state2 = state(vector(6, -1, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  vector_t force1; vector_t tau1; vector_t force2; vector_t tau2;
  exert_force(body1, body2, f, state1, state2, &force1, &force2, &tau1, &tau2);
  munit_assert_double_equal(force1.x,  10, 6);
  munit_assert_double_equal(force2.x, -10, 6);
  munit_assert_double_equal(tau1.z, -10, 6);
  munit_assert_double_equal(tau2.z,  20, 6);
  return MUNIT_OK;
}

MunitTest test_force[] = {
  {"/gravitation"       , test_gravitation       , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/exert_gravitation" , test_exert_gravitation , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/fixed_force"       , test_fixed_force       , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/exert_fixed_force" , test_exert_fixed_force , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotate_fixed_force", test_rotate_fixed_force, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/spring_damper"     , test_spring_damper     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
  {"/spring_neutral"    , test_spring_neutral    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/spring_tension"    , test_spring_tension    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/spring_torque"     , test_spring_torque     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                 , NULL                   , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
