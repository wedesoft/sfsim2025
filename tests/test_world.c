#include "sfsim2025/world.h"
#include "sfsim2025/mechanics.h"
#include "test_world.h"
#include "test_helper.h"


static MunitResult test_create(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  munit_assert_not_null(world);
  return MUNIT_OK;
}

static MunitResult test_no_states_initially(const MunitParameter params[], void *data) {
  munit_assert_int(make_world()->states.size, ==, 0);
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

static world_info_t world_info(void) {
  world_info_t result = make_world_info();
  append_body(&result.bodies, body(2.0, diagonal(1, 1, 1)));
  append_forces(&result.forces, forces(vector(1, 2, 3), vector(0, 0, 0)));
  append_pointer(&result.rigid_bodies, make_rigid_body());
  return result;
}

static MunitResult test_position_change(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_pointer(&world->states, state(vector(0, 0, 0), vector(1, 2, 3), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  world_info_t info = world_info();
  world_t *changed = world_change(0, 2, world, &info);
  state_t *result = get_pointer(changed->states)[0];
  munit_assert_double(result->position.x, ==, 2);
  munit_assert_double(result->position.y, ==, 4);
  munit_assert_double(result->position.z, ==, 6);
  return MUNIT_OK;
}

static MunitResult test_speed_change(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_pointer(&world->states, state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  world_info_t info = world_info();
  world_t *changed = world_change(0, 4, world, &info);
  state_t *result = get_pointer(changed->states)[0];
  munit_assert_double(result->speed.x, ==, 2);
  munit_assert_double(result->speed.y, ==, 4);
  munit_assert_double(result->speed.z, ==, 6);
  return MUNIT_OK;
}

static world_info_t world_info2(void) {
  world_info_t result = make_world_info();
  append_body(&result.bodies, body(1.0, diagonal(1, 2, 2)));
  append_forces(&result.forces, forces(vector(1, 2, 3), vector(1, 2, 3)));
  append_pointer(&result.rigid_bodies, make_rigid_body());
  return result;
}

static MunitResult test_rotation(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_pointer(&world->states, state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  world_info_t info = world_info2();
  world_t *changed = world_change(0, 2, world, &info);
  state_t *result = get_pointer(changed->states)[0];
  munit_assert_double(result->rotation.x, ==, 2);
  munit_assert_double(result->rotation.y, ==, 2);
  munit_assert_double(result->rotation.z, ==, 3);
  return MUNIT_OK;
}

static world_info_t world_info3(void) {
  world_info_t result = make_world_info();
  append_body(&result.bodies, body(1.0, diagonal(1, 1, 1)));
  append_forces(&result.forces, forces(vector(1, 2, 3), vector(1, 2, 3)));
  append_pointer(&result.rigid_bodies, make_rigid_body());
  return result;
}

static MunitResult test_orientation_change(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_pointer(&world->states, state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0.1, 0.2, 0.3)));
  world_info_t info = world_info3();
  world_t *changed = world_change(0, 2, world, &info);
  state_t *result = get_pointer(changed->states)[0];
  munit_assert_double_equal(result->orientation.b, 0.1, 6);
  return MUNIT_OK;
}

static world_info_t world_info4(void) {
  world_info_t result = make_world_info();
  append_body(&result.bodies, body(1.0, diagonal(1, 2, 4)));
  append_forces(&result.forces, forces(vector(0, 0, 0), vector(0, 0, 0)));
  append_pointer(&result.rigid_bodies, make_rigid_body());
  return result;
}

static MunitResult test_newton_euler(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_pointer(&world->states, state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(1, 1, 1)));
  world_info_t info = world_info4();
  world_t *changed = world_change(0, 2, world, &info);
  state_t *result = get_pointer(changed->states)[0];
  munit_assert_double(result->rotation.x, ==, -4.0);
  munit_assert_double(result->rotation.y, ==,  3.0);
  munit_assert_double(result->rotation.z, ==, -0.5);
  return MUNIT_OK;
}

static MunitResult test_consider_orientation(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_pointer(&world->states, state(vector(0, 0, 0), vector(0, 0, 0), quaternion(-1, 0, 0, 0), vector(0.1, 0.2, 0.3)));
  world_info_t info = world_info3();
  world_t *changed = world_change(0, 2, world, &info);
  state_t *result = get_pointer(changed->states)[0];
  munit_assert_double_equal(result->orientation.b, -0.1, 6);
  return MUNIT_OK;
}

static world_info_t world_info5(void) {
  world_info_t result = make_world_info();
  append_body(&result.bodies, body(1.0, diagonal(1, 1, 1)));
  append_forces(&result.forces, forces(vector(0, 0, 0), vector(0, 0, 0)));
  append_pointer(&result.rigid_bodies, make_rigid_body());
  append_body(&result.bodies, body(1.0, diagonal(1, 1, 1)));
  append_forces(&result.forces, forces(vector(0, 0, 0), vector(0, 0, 0)));
  append_pointer(&result.rigid_bodies, make_rigid_body());
  append_joint(&result.joints, ball_in_socket(0, 1, vector(2, 0, 0), vector(-2, 0, 0)));
  return result;
}

static MunitResult test_joint(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_pointer(&world->states, state(vector(-2, 0, 0), vector(-1, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  append_pointer(&world->states, state(vector(+2, 0, 0), vector(+1, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  world_info_t info = world_info5();
  world_t *changed = world_change(0, 1, world, &info);
  state_t *result1 = get_pointer(changed->states)[0];
  state_t *result2 = get_pointer(changed->states)[1];
  munit_assert_double(result1->speed.x, ==, +1);
  munit_assert_double(result2->speed.x, ==, -1);
  return MUNIT_OK;
}

static void add_cube_faces(rigid_body_t *body) {
  add_face(body, face(0, 1, 3));
  add_face(body, face(0, 3, 2));
  add_face(body, face(5, 4, 7));
  add_face(body, face(5, 6, 7));
  add_face(body, face(4, 0, 2));
  add_face(body, face(4, 2, 6));
  add_face(body, face(1, 5, 7));
  add_face(body, face(1, 7, 3));
  add_face(body, face(0, 5, 1));
  add_face(body, face(0, 4, 5));
  add_face(body, face(2, 3, 7));
  add_face(body, face(2, 7, 6));
}

static rigid_body_t *make_cube(double w2, double h2, double d2) {
  rigid_body_t *result = make_rigid_body();
  add_point(result, vector(-w2, -h2, -d2));
  add_point(result, vector(+w2, -h2, -d2));
  add_point(result, vector(-w2, -h2, +d2));
  add_point(result, vector(+w2, -h2, +d2));
  add_point(result, vector(-w2, +h2, -d2));
  add_point(result, vector(+w2, +h2, -d2));
  add_point(result, vector(-w2, +h2, +d2));
  add_point(result, vector(+w2, +h2, +d2));
  add_cube_faces(result);
  return result;
}

static world_info_t world_info6(void) {
  world_info_t result = make_world_info();
  result.iterations = 10;
  result.restitution = 0.5;
  append_body(&result.bodies, body(5.9742e+24, inertia_sphere(5.9742e+24, 6370000)));
  append_forces(&result.forces, forces(vector(0, 0, 0), vector(0, 0, 0)));
  append_pointer(&result.rigid_bodies, make_cube(6370000, 6370000, 6370000));
  append_body(&result.bodies, body(1.0, inertia_cuboid(1.0, 1, 1, 1)));
  append_forces(&result.forces, forces(vector(0, -9.81, 0), vector(0, 0, 0)));
  append_pointer(&result.rigid_bodies, make_cube(0.5, 0.5, 0.5));
  return result;
}

static MunitResult test_contact(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_pointer(&world->states, state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  append_pointer(&world->states, state(vector(0, 0.5, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  world_info_t info = world_info6();
  world_t *changed = world_change(0, 1, world, &info);
  state_t *result2 = get_pointer(changed->states)[1];
  munit_assert_double_equal(result2->speed.y, 0, 5);
  return MUNIT_OK;
}

static MunitResult test_collision(const MunitParameter params[], void *data) {
  world_t *world = make_world();
  append_pointer(&world->states, state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  append_pointer(&world->states, state(vector(0, 0.5, 0), vector(0, -0.5, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  world_info_t info = world_info6();
  world_t *changed = world_change(0, 0, world, &info);
  state_t *result2 = get_pointer(changed->states)[1];
  munit_assert_double_equal(result2->speed.y, 0.75, 5);
  return MUNIT_OK;
}

MunitTest test_world[] = {
  {"/create"              , test_create              , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_states_initially" , test_no_states_initially , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale_states"        , test_scale_states        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_states"          , test_add_states          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/position_change"     , test_position_change     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/speed_change"        , test_speed_change        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/rotation"            , test_rotation            , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/orientation_change"  , test_orientation_change  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/newton_euler"        , test_newton_euler        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/consider_orientation", test_consider_orientation, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/joint"               , test_joint               , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/contact"             , test_contact             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/collision"           , test_collision           , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                   , NULL                     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
