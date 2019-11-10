#include "sfsim2025/state.h"
#include "test_state.h"
#include "test_helper.h"


static MunitResult test_components(const MunitParameter params[], void *data) {
  state_t *result = state(vector(2, 3, 5), vector(3, 5, 7), quaternion(1, 0, 0, 0), vector(5, 7, 11));
  munit_assert_double(result->position.x, ==, 2);
  munit_assert_double(result->speed.x, ==, 3);
  munit_assert_double(result->orientation.a, ==, 1);
  munit_assert_double(result->rotation.x, ==, 5);
  return MUNIT_OK;
}

static MunitResult test_scale_state(const MunitParameter params[], void *data) {
  state_t *s = state(vector(1, 0, 0), vector(2, 0, 0), quaternion(-1, 0, 0, 0), vector(0.5, 0, 0));
  state_t *result = scale_state(s, 2);
  munit_assert_double(result->position.x, ==, 2);
  munit_assert_double(result->speed.x, ==, 4);
  munit_assert_double(result->orientation.a, ==, -2);
  munit_assert_double(result->rotation.x, ==, 1.0);
  return MUNIT_OK;
}

static MunitResult test_add_states(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(1, 0, 0), vector(3, 0, 0), quaternion(-1, 0, 0, 0), vector(0.4, 0, 0));
  state_t *s2 = state(vector(2, 0, 0), vector(4, 0, 0), quaternion( 2, 0, 0, 0), vector(0.1, 0, 0));
  state_t *result = add_states(s1, s2);
  munit_assert_double(result->position.x, ==, 3);
  munit_assert_double(result->speed.x, ==, 7);
  munit_assert_double(result->orientation.a, ==, 1);
  munit_assert_double(result->rotation.x, ==, 0.5);
  return MUNIT_OK;
}

static MunitResult test_force_changes_speed(const MunitParameter params[], void *data) {
  forces_t f = forces(vector(2, 3, 5), vector(0, 0, 0));
  body_t b = body(0.5, diagonal(1, 1, 1));
  vector_t s = speed_change(f, b, vector(0, 0, 0), 3);
  munit_assert_double(s.x, ==, 12);
  munit_assert_double(s.y, ==, 18);
  munit_assert_double(s.z, ==, 30);
  return MUNIT_OK;
}

static MunitResult test_impulse_changes_speed(const MunitParameter params[], void *data) {
  forces_t f = forces(vector(0, 0, 0), vector(0, 0, 0));
  body_t b = body(0.5, diagonal(1, 1, 1));
  vector_t s = speed_change(f, b, vector(2, 3, 5), 3);
  munit_assert_double(s.x, ==,  4);
  munit_assert_double(s.y, ==,  6);
  munit_assert_double(s.z, ==, 10);
  return MUNIT_OK;
}

MunitTest test_state[] = {
  {"/components"           , test_components           , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_state"          , test_scale_state          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_states"           , test_add_states           , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/force_changes_speed"  , test_force_changes_speed  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/impulse_changes_speed", test_impulse_changes_speed, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                    , NULL                      , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
