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

MunitTest test_world[] = {
  {"/create"             , test_create             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_bodies_initially", test_no_bodies_initially, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_states_initially", test_no_states_initially, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_joints_initially", test_no_joints_initially, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_copies_bodies", test_scale_copies_bodies, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_copies_joints", test_scale_copies_joints, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                  , NULL                    , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
