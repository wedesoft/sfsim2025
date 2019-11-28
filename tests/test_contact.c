#include "sfsim2025/contact.h"
#include "sfsim2025/mechanics.h"
#include "test_contact.h"


static MunitResult test_body_indices(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4);
  munit_assert_int(c.i, ==, 2);
  munit_assert_int(c.j, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_normal(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4);
  munit_assert_double(c.normal.x, ==,  5);
  munit_assert_double(c.normal.y, ==,  7);
  munit_assert_double(c.normal.z, ==, 11);
  return MUNIT_OK;
}

static MunitResult test_point(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4);
  munit_assert_double(c.point.x, ==, 1);
  munit_assert_double(c.point.y, ==, 2);
  munit_assert_double(c.point.z, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_distance(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4);
  munit_assert_double(c.distance, ==, 0.1);
  return MUNIT_OK;
}

static MunitResult test_restitution_value(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4);
  munit_assert_double(c.restitution, ==, 0.4);
  return MUNIT_OK;
}

static MunitResult test_relative_speed(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0.25), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(3, 0, 0), vector(0, 0, -0.25), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  vector_t v_relative = relative_speed(s1, s2, vector(1.5, 0, 0));
  munit_assert_double(v_relative.x, ==,  0  );
  munit_assert_double(v_relative.y, ==,  0  );
  munit_assert_double(v_relative.z, ==, -0.5);
  return MUNIT_OK;
}

static MunitResult test_relative_rotation(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 1, 0));
  state_t *s2 = state(vector(3, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 1));
  vector_t v_relative = relative_speed(s1, s2, vector(1.5, 0, 0));
  munit_assert_double(v_relative.x, ==,  0  );
  munit_assert_double(v_relative.y, ==, -1.5);
  munit_assert_double(v_relative.z, ==,  1.5);
  return MUNIT_OK;
}

static MunitResult test_jacobian_linear(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(3, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), 0.1, 0.4);
  large_matrix_t j = contact_jacobian(c, s1, s2);
  munit_assert_int(j.rows, ==, 1);
  munit_assert_int(j.cols, ==, 12);
  munit_assert_double(j.data[0], ==,  0);
  munit_assert_double(j.data[1], ==,  0);
  munit_assert_double(j.data[2], ==, -1);
  munit_assert_double(j.data[6], ==,  0);
  munit_assert_double(j.data[7], ==,  0);
  munit_assert_double(j.data[8], ==,  1);
  return MUNIT_OK;
}

static MunitResult test_jacobian_angular(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(3, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), 0.1, 0.4);
  large_matrix_t j = contact_jacobian(c, s1, s2);
  munit_assert_int(j.rows, ==, 1);
  munit_assert_int(j.cols, ==, 12);
  munit_assert_double(j.data[ 3], ==, -2);
  munit_assert_double(j.data[ 4], ==,  1);
  munit_assert_double(j.data[ 5], ==,  0);
  munit_assert_double(j.data[ 9], ==,  2);
  munit_assert_double(j.data[10], ==,  2);
  munit_assert_double(j.data[11], ==,  0);
  return MUNIT_OK;
}

static MunitResult test_no_correction(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(3, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), 0.1, 0.0);
  large_vector_t b = contact_correction(c, s1, s2);
  munit_assert_int(b.rows, ==, 1);
  munit_assert_double(b.data[0], ==, 0.0);
  return MUNIT_OK;
}

static MunitResult test_correction(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(3, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), -0.1, 0.0);
  large_vector_t b = contact_correction(c, s1, s2);
  munit_assert_int(b.rows, ==, 1);
  munit_assert_double(b.data[0], ==, -0.1);
  return MUNIT_OK;
}

static MunitResult test_restitution(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0.25), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(3, 0, 0), vector(0, 0, -0.25), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), 0.1, 0.5);
  large_vector_t b = contact_correction(c, s1, s2);
  munit_assert_int(b.rows, ==, 1);
  munit_assert_double(b.data[0], ==, -0.25);
  return MUNIT_OK;
}

static MunitResult test_contact_impulse(const MunitParameter params[], void *data) {
  body_t body1 = body(5.9722e+24, inertia_sphere(5.9722e+24, 6370000));
  body_t body2 = body(1.0, inertia_cuboid(1.0, 0.1, 2, 0.1));
  state_t *s1 = state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 1, 0), vector(0, -0.01, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(0, 1, vector(0, 1, 0), vector(0, 0, 0), 0, 0);
  vector_t p1; vector_t t1; vector_t p2; vector_t t2;
  contact_impulse(body1, body2, s1, s2, c, &p1, &p2, &t1, &t2);
  munit_assert_double_equal(p1.x,  0.0 , 6);
  munit_assert_double_equal(p1.y, -0.01, 6);
  munit_assert_double_equal(p1.z,  0.0 , 6);
  munit_assert_double_equal(p2.x,  0.0 , 6);
  munit_assert_double_equal(p2.y,  0.01, 6);
  munit_assert_double_equal(p2.z,  0.0 , 6);
  return MUNIT_OK;
}

static MunitResult test_separating_objects(const MunitParameter params[], void *data) {
  body_t body1 = body(5.9722e+24, inertia_sphere(5.9722e+24, 6370000));
  body_t body2 = body(1.0, inertia_cuboid(1.0, 0.1, 2, 0.1));
  state_t *s1 = state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 1, 0), vector(0, 0.01, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(0, 1, vector(0, 1, 0), vector(0, 0, 0), 0, 0);
  vector_t p1; vector_t t1; vector_t p2; vector_t t2;
  contact_impulse(body1, body2, s1, s2, c, &p1, &p2, &t1, &t2);
  munit_assert_double_equal(p1.x, 0.0, 6);
  munit_assert_double_equal(p1.y, 0.0, 6);
  munit_assert_double_equal(p1.z, 0.0, 6);
  munit_assert_double_equal(p2.x, 0.0, 6);
  munit_assert_double_equal(p2.y, 0.0, 6);
  munit_assert_double_equal(p2.z, 0.0, 6);
  return MUNIT_OK;
}

MunitTest test_contact[] = {
  {"/body_indices"      , test_body_indices      , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/normal"            , test_normal            , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/point"             , test_point             , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/distance"          , test_distance          , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/restitution_value" , test_restitution_value , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/relative_speed"    , test_relative_speed    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/relative_rotation" , test_relative_rotation , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/jacobian_linear"   , test_jacobian_linear   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/jacobian_angular"  , test_jacobian_angular  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_correction"     , test_no_correction     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/correction"        , test_correction        , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/restitution"       , test_restitution       , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/contact_impulse"   , test_contact_impulse   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/separating_objects", test_separating_objects, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                 , NULL                   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
