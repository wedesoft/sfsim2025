#include "sfsim2025/contact.h"
#include "sfsim2025/mechanics.h"
#include "test_contact.h"


static MunitResult test_body_indices(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4, 2.0, uniform_friction(1.0));
  munit_assert_int(c.i, ==, 2);
  munit_assert_int(c.j, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_normal(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4, 2.0, uniform_friction(1.0));
  munit_assert_double(c.normal.x, ==,  5);
  munit_assert_double(c.normal.y, ==,  7);
  munit_assert_double(c.normal.z, ==, 11);
  return MUNIT_OK;
}

static MunitResult test_point(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4, 2.0, uniform_friction(1.0));
  munit_assert_double(c.point.x, ==, 1);
  munit_assert_double(c.point.y, ==, 2);
  munit_assert_double(c.point.z, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_distance(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4, 2.0, uniform_friction(1.0));
  munit_assert_double(c.distance, ==, 0.1);
  return MUNIT_OK;
}

static MunitResult test_restitution_value(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4, 2.0, uniform_friction(1.0));
  munit_assert_double(c.restitution, ==, 0.4);
  return MUNIT_OK;
}

static MunitResult test_speed_value(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(5, 7, 11), vector(1, 2, 3), 0.1, 0.4, 2.0, uniform_friction(1.0));
  munit_assert_double(c.normal_speed, ==, 2.0);
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
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), 0.1, 0.4, 2.0, uniform_friction(1.0));
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
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), 0.1, 0.4, 2.0, uniform_friction(1.0));
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

static MunitResult test_positive_correction(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), 0.1, 0.0, 2.0, uniform_friction(1.0));
  large_vector_t b = contact_correction(c, false);
  munit_assert_int(b.rows, ==, 1);
  munit_assert_double(b.data[0], ==, 0.1);
  return MUNIT_OK;
}

static MunitResult test_negative_correction(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), -0.1, 0.0, -2.0, uniform_friction(1.0));
  large_vector_t b = contact_correction(c, false);
  munit_assert_int(b.rows, ==, 1);
  munit_assert_double(b.data[0], ==, -0.1);
  return MUNIT_OK;
}

static MunitResult test_restitution(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), 0.1, 0.5, -2.0, uniform_friction(1.0));
  large_vector_t b = contact_correction(c, true);
  munit_assert_int(b.rows, ==, 1);
  munit_assert_double(b.data[0], ==, -1);
  return MUNIT_OK;
}

static MunitResult test_ortho_vector(const MunitParameter params[], void *data) {
  quaternion_t q = quaternion(1, 0, 0, 0);
  vector_t m;
  vector_t o;
  friction_tangentials(q, vector(1, 0, 0), &m, &o);
  munit_assert_double_equal(m.x, 0, 6); munit_assert_double_equal(m.y, 1, 6); munit_assert_double_equal(m.z, 0, 6);
  munit_assert_double_equal(o.x, 0, 6); munit_assert_double_equal(o.y, 0, 6); munit_assert_double_equal(o.z, 1, 6);
  return MUNIT_OK;
}

static MunitResult test_ortho_vector2(const MunitParameter params[], void *data) {
  quaternion_t q = quaternion_rotation(-M_PI / 2, vector(1, 0, 0));
  vector_t m;
  vector_t o;
  friction_tangentials(q, vector(1, 0, 0), &m, &o);
  munit_assert_double_equal(m.x, 0, 6); munit_assert_double_equal(m.y, 0, 6); munit_assert_double_equal(m.z, -1, 6);
  munit_assert_double_equal(o.x, 0, 6); munit_assert_double_equal(o.y, 1, 6); munit_assert_double_equal(o.z,  0, 6);
  return MUNIT_OK;
}

static MunitResult test_ortho_vector3(const MunitParameter params[], void *data) {
  quaternion_t q = quaternion_rotation(-M_PI / 2, vector(0, 1, 0));
  vector_t m;
  vector_t o;
  friction_tangentials(q, vector(1, 0, 0), &m, &o);
  munit_assert_double_equal(m.x, 0, 6); munit_assert_double_equal(m.y, 1, 6); munit_assert_double_equal(m.z, 0, 6);
  munit_assert_double_equal(o.x, 0, 6); munit_assert_double_equal(o.y, 0, 6); munit_assert_double_equal(o.z, 1, 6);
  return MUNIT_OK;
}

static MunitResult test_friction_jacobian(const MunitParameter params[], void *data) {
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(3, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), 0.1, 0.4, 2.0, uniform_friction(1.0));
  large_matrix_t j = friction_jacobian(c, s1, s2);
  munit_assert_int(j.rows, ==, 2);
  munit_assert_int(j.cols, ==, 12);
  munit_assert_double(j.data[ 0], ==,  0);
  munit_assert_double(j.data[ 1], ==, -1);
  munit_assert_double(j.data[ 2], ==,  0);
  munit_assert_double(j.data[12], ==,  1);
  munit_assert_double(j.data[13], ==,  0);
  munit_assert_double(j.data[14], ==,  0);
  munit_assert_double(j.data[ 3], ==,  3);
  munit_assert_double(j.data[ 4], ==,  0);
  munit_assert_double(j.data[ 5], ==, -1);
  munit_assert_double(j.data[15], ==,  0);
  munit_assert_double(j.data[16], ==,  3);
  munit_assert_double(j.data[17], ==, -2);
  munit_assert_double(j.data[ 6], ==,  0);
  munit_assert_double(j.data[ 7], ==,  1);
  munit_assert_double(j.data[ 8], ==,  0);
  munit_assert_double(j.data[18], ==, -1);
  munit_assert_double(j.data[19], ==,  0);
  munit_assert_double(j.data[20], ==,  0);
  munit_assert_double(j.data[ 9], ==, -3);
  munit_assert_double(j.data[10], ==,  0);
  munit_assert_double(j.data[11], ==, -2);
  munit_assert_double(j.data[21], ==,  0);
  munit_assert_double(j.data[22], ==, -3);
  munit_assert_double(j.data[23], ==,  2);
  return MUNIT_OK;
}

static MunitResult test_friction_correction(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(0, 0, 1), vector(1, 2, 3), 0.1, 0.4, 2.0, uniform_friction(1.0));
  large_vector_t b = friction_correction(c);
  munit_assert_int(b.rows, ==, 2);
  munit_assert_double(b.data[0], ==, 0);
  munit_assert_double(b.data[1], ==, 0);
  return MUNIT_OK;
}

static MunitResult test_contact_impulse(const MunitParameter params[], void *data) {
  body_t body1 = body(5.9722e+24, inertia_sphere(5.9722e+24, 6370000));
  body_t body2 = body(1.0, inertia_cuboid(1.0, 0.1, 2, 0.1));
  state_t *s1 = state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 1, 0), vector(0, -0.01, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(0, 1, vector(0, 1, 0), vector(0, 0, 0), 0, 0.5, -2.0, uniform_friction(1.0));
  vector_t p1; vector_t t1; vector_t p2; vector_t t2; double lambda_;
  contact_impulse(body1, body2, s1, s2, c, false, &p1, &p2, &t1, &t2, &lambda_);
  munit_assert_double_equal(p1.x,  0.0 , 6);
  munit_assert_double_equal(p1.y, -0.01, 6);
  munit_assert_double_equal(p1.z,  0.0 , 6);
  munit_assert_double_equal(p2.x,  0.0 , 6);
  munit_assert_double_equal(p2.y,  0.01, 6);
  munit_assert_double_equal(p2.z,  0.0 , 6);
  munit_assert_double_equal(lambda_, 0.01, 6);
  return MUNIT_OK;
}

static MunitResult test_separating_objects(const MunitParameter params[], void *data) {
  body_t body1 = body(5.9722e+24, inertia_sphere(5.9722e+24, 6370000));
  body_t body2 = body(1.0, inertia_cuboid(1.0, 0.1, 2, 0.1));
  state_t *s1 = state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 1, 0), vector(0, 0.01, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(0, 1, vector(0, 1, 0), vector(0, 0, 0), 0, 0, 0.01, uniform_friction(1.0));
  vector_t p1; vector_t t1; vector_t p2; vector_t t2; double lambda_;
  contact_impulse(body1, body2, s1, s2, c, false, &p1, &p2, &t1, &t2, &lambda_);
  munit_assert_double_equal(p1.x, 0.0, 6);
  munit_assert_double_equal(p1.y, 0.0, 6);
  munit_assert_double_equal(p1.z, 0.0, 6);
  munit_assert_double_equal(p2.x, 0.0, 6);
  munit_assert_double_equal(p2.y, 0.0, 6);
  munit_assert_double_equal(p2.z, 0.0, 6);
  munit_assert_double_equal(lambda_, 0.0, 6);
  return MUNIT_OK;
}

static MunitResult test_collision(const MunitParameter params[], void *data) {
  body_t body1 = body(5.9722e+24, inertia_sphere(5.9722e+24, 6370000));
  body_t body2 = body(1.0, inertia_cuboid(1.0, 0.1, 2, 0.1));
  state_t *s1 = state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 1, 0), vector(0, -0.1, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(0, 1, vector(0, 1, 0), vector(0, 0, 0), 0, 0.5, -0.1, uniform_friction(1.0));
  vector_t p1; vector_t t1; vector_t p2; vector_t t2; double lambda_;
  contact_impulse(body1, body2, s1, s2, c, true, &p1, &p2, &t1, &t2, &lambda_);
  munit_assert_double_equal(p2.x,  0.0 , 6);
  munit_assert_double_equal(p2.y,  0.15, 6);
  munit_assert_double_equal(p2.z,  0.0 , 6);
  munit_assert_double_equal(lambda_, 0.15, 6);
  return MUNIT_OK;
}

static MunitResult test_is_resting(const MunitParameter params[], void *data) {
  contact_t c = contact(0, 1, vector(0, 1, 0), vector(0, 0, 0), 0, 0.2, -0.2, uniform_friction(1.0));
  munit_assert_true(is_resting(c, 0.1));
  return MUNIT_OK;
}

static MunitResult test_is_not_resting(const MunitParameter params[], void *data) {
  contact_t c = contact(0, 1, vector(0, 1, 0), vector(0, 0, 0), 0, 0.2, -0.6, uniform_friction(1.0));
  munit_assert_false(is_resting(c, 0.1));
  return MUNIT_OK;
}

static MunitResult test_is_not_resting2(const MunitParameter params[], void *data) {
  contact_t c = contact(0, 1, vector(0, 1, 0), vector(0, 0, 0), 0, 0.2, 0.6, uniform_friction(1.0));
  munit_assert_false(is_resting(c, 0.1));
  return MUNIT_OK;
}

static MunitResult test_friction(const MunitParameter params[], void *data) {
  body_t body1 = body(5.9722e+24, inertia_sphere(5.9722e+24, 6370000));
  body_t body2 = body(1.0, inertia_cuboid(1.0, 0.1, 2, 0.1));
  state_t *s1 = state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 1, 0), vector(0.1, 0.0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(0, 1, vector(0, 1, 0), vector(0, 1, 0), 0, 0.5, -0.1, uniform_friction(1.0));
  vector_t p1; vector_t t1; vector_t p2; vector_t t2;
  friction_impulse(body1, body2, s1, s2, c, &p1, &p2, &t1, &t2, 1.0);
  munit_assert_double_equal(p2.x, -0.1, 6);
  munit_assert_double_equal(p2.y,  0.0, 6);
  munit_assert_double_equal(p2.z,  0. , 6);
  return MUNIT_OK;
}

static MunitResult test_friction_limit(const MunitParameter params[], void *data) {
  body_t body1 = body(5.9722e+24, inertia_sphere(5.9722e+24, 6370000));
  body_t body2 = body(1.0, inertia_cuboid(1.0, 0.1, 2, 0.1));
  state_t *s1 = state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 1, 0), vector(1.0, 0.0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  contact_t c = contact(0, 1, vector(0, 1, 0), vector(0, 1, 0), 0, 0.5, -0.1, uniform_friction(0.5));
  vector_t p1; vector_t t1; vector_t p2; vector_t t2;
  friction_impulse(body1, body2, s1, s2, c, &p1, &p2, &t1, &t2, 1.0);
  munit_assert_double_equal(p2.x, -0.5, 6);
  munit_assert_double_equal(p2.y,  0.0, 6);
  munit_assert_double_equal(p2.z,  0.0, 6);
  return MUNIT_OK;
}

MunitTest test_contact[] = {
  {"/body_indices"       , test_body_indices       , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/normal"             , test_normal             , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/point"              , test_point              , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/distance"           , test_distance           , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/restitution_value"  , test_restitution_value  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/speed_value"        , test_speed_value        , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/relative_speed"     , test_relative_speed     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/relative_rotation"  , test_relative_rotation  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/jacobian_linear"    , test_jacobian_linear    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/jacobian_angular"   , test_jacobian_angular   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/positive_correction", test_positive_correction, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/negative_correction", test_negative_correction, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/restitution"        , test_restitution        , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/ortho_vector"       , test_ortho_vector       , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/ortho_vector2"      , test_ortho_vector2      , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/ortho_vector3"      , test_ortho_vector3      , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/friction_jacobian"  , test_friction_jacobian  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/friction_correction", test_friction_correction, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/contact_impulse"    , test_contact_impulse    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/separating_objects" , test_separating_objects , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/collision"          , test_collision          , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/is_resting"         , test_is_resting         , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/is_not_resting"     , test_is_not_resting     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/is_not_resting2"    , test_is_not_resting2    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/friction"           , test_friction           , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/friction_limit"     , test_friction_limit     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                  , NULL                    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
