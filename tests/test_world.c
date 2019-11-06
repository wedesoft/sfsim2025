#include "sfsim2025/world.h"
#include "test_world.h"
#include "test_helper.h"


static MunitResult test_create(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  munit_assert_not_null(world);
  return MUNIT_OK;
}

static MunitResult test_no_bodies_initially(const MunitParameter params[], void *data) {
  munit_assert_int(make_world()->bodies.size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_no_states_initially(const MunitParameter params[], void *data) {
  munit_assert_int(make_world()->states.size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_no_joints_initially(const MunitParameter params[], void *data) {
  munit_assert_int(make_world()->joints.size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_scale_copies_bodies(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_body(&world->bodies, body(2, diagonal(3, 5, 7)));
  world_t *scaled = scale_world(world, 2.0);
  munit_assert_int(scaled->bodies.size, ==, 1);
  munit_assert_double(get_body(scaled->bodies)[0].mass, ==, 2.0);
  return MUNIT_OK;
}

static MunitResult test_scale_copies_joints(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_joint(&world->joints, joint(2, 3, vector(0, 0, 0), vector(0, 0, 0)));
  world_t *scaled = scale_world(world, 2.0);
  munit_assert_int(scaled->joints.size, ==, 1);
  munit_assert_int(get_joint(scaled->joints)[0].i, ==, 2);
  munit_assert_int(get_joint(scaled->joints)[0].j, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_scale_states(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_pointer(&world->states, state(vector(1, 0, 0), vector(2, 0, 0), quaternion(-1, 0, 0, 0), vector(0.5, 0, 0)));
  world_t *scaled = scale_world(world, 2.0);
  munit_assert_int(scaled->states.size, ==, 1);
  state_t *result = get_pointer(scaled->states)[0];
  munit_assert_double(result->position.x, ==, 2);
  munit_assert_double(result->speed.x, ==, 4);
  munit_assert_double(result->orientation.a, ==, -2);
  munit_assert_double(result->rotation.x, ==, 1.0);
  return MUNIT_OK;
}

static MunitResult test_add_copies_bodies(const MunitParameter params[], void *data) {
  world_t *w1 = make_world();
  world_t *w2 = make_world();
  append_body(&w1->bodies, body(2, diagonal(3, 5, 7)));
  world_t *added = add_worlds(w1, w2);
  munit_assert_int(added->bodies.size, ==, 1);
  munit_assert_double(get_body(added->bodies)[0].mass, ==, 2.0);
  return MUNIT_OK;
}

static MunitResult test_add_copies_joints(const MunitParameter params[], void *data) {
  world_t *w1 = make_world();
  world_t *w2 = make_world();
  append_joint(&w1->joints, joint(2, 3, vector(0, 0, 0), vector(0, 0, 0)));
  world_t *added = add_worlds(w1, w2);
  munit_assert_int(added->joints.size, ==, 1);
  munit_assert_int(get_joint(added->joints)[0].i, ==, 2);
  munit_assert_int(get_joint(added->joints)[0].j, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_add_states(const MunitParameter params[], void *data) {
  world_t *w1 = make_world();
  world_t *w2 = make_world();
  state_t *s1 = state(vector(1, 0, 0), vector(3, 0, 0), quaternion(-1, 0, 0, 0), vector(0.4, 0, 0));
  state_t *s2 = state(vector(2, 0, 0), vector(4, 0, 0), quaternion( 2, 0, 0, 0), vector(0.1, 0, 0));
  append_pointer(&w1->states, s1);
  append_pointer(&w2->states, s2);
  world_t *added = add_worlds(w1, w2);
  munit_assert_int(added->states.size, ==, 1);
  state_t *result = get_pointer(added->states)[0];
  munit_assert_double(result->position.x, ==, 3);
  munit_assert_double(result->speed.x, ==, 7);
  munit_assert_double(result->orientation.a, ==, 1);
  munit_assert_double(result->rotation.x, ==, 0.5);
  return MUNIT_OK;
}

MunitTest test_world[] = {
  {"/create"             , test_create             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_bodies_initially", test_no_bodies_initially, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_states_initially", test_no_states_initially, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_joints_initially", test_no_joints_initially, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_copies_bodies", test_scale_copies_bodies, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_copies_joints", test_scale_copies_joints, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_states"       , test_scale_states       , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_copies_bodies"  , test_add_copies_bodies  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_copies_joints"  , test_add_copies_joints  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_states"         , test_add_states         , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                  , NULL                    , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
