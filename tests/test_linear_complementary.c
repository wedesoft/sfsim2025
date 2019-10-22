#include "sfsim2025/linear_complementary.h"
#include "sfsim2025/mechanics.h"
#include "test_linear_complementary.h"
#include "test_helper.h"


static MunitResult test_one_contact(const MunitParameter params[], void *data) {
  body_info_t info1 = body_info(1.0, inertia_cuboid(1.0, 1, 0.4, 0.2), vector(0, 0, -9.81), vector(0, 0, 0));
  body_info_t info2 = body_info(6e+24, inertia_cuboid(6e+24, 6370e+3, 6370e+3, 6370e+3), vector(0, 0, 0), vector(0, 0, 0));
  list_t body_infos = make_list(); append_body_info(&body_infos, info1); append_body_info(&body_infos, info2);
  state_t *s1 = state(vector(0, 0, 6370e+3 + 0.1), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  list_t states = make_list(); append_pointer(&states, s1); append_pointer(&states, s2);
  contact_t c = contact(0, 1, vector(0, 0, -1), vector(0, 0, 6370e+3));
  list_t contacts = make_list(); append_contact(&contacts, c);
  large_matrix_t a; large_vector_t b;
  frictionless_case(body_infos, states, contacts, &a, &b, 0.5);
  munit_assert_int(a.rows, ==, 1);
  munit_assert_int(a.cols, ==, 1);
  munit_assert_double_equal(a.data[0], 1.0, 6);
  munit_assert_int(b.rows, ==, 1);
  munit_assert_double_equal(b.data[0], -4.905, 6);
  return MUNIT_OK;
}

static MunitResult test_two_contacts(const MunitParameter params[], void *data) {
  body_info_t info1 = body_info(1.0, inertia_cuboid(1.0, 1, 0.4, 0.2), vector(0, 0, -9.81), vector(0, 0, 0));
  body_info_t info2 = body_info(6e+24, inertia_cuboid(6e+24, 6370e+3, 6370e+3, 6370e+3), vector(0, 0, 0), vector(0, 0, 0));
  list_t body_infos = make_list(); append_body_info(&body_infos, info1); append_body_info(&body_infos, info2);
  state_t *s1 = state(vector(0, 0, 6370e+3 + 0.1), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 0, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  list_t states = make_list(); append_pointer(&states, s1); append_pointer(&states, s2);
  contact_t c1 = contact(0, 1, vector(0, 0, -1), vector(-1, 0, 6370e+3));
  contact_t c2 = contact(0, 1, vector(0, 0, -1), vector(+1, 0, 6370e+3));
  list_t contacts = make_list(); append_contact(&contacts, c1); append_contact(&contacts, c2);
  large_matrix_t a; large_vector_t b;
  frictionless_case(body_infos, states, contacts, &a, &b, 1.0);
  munit_assert_int(a.rows, ==, 2);
  munit_assert_int(a.cols, ==, 2);
  munit_assert_double_equal(a.data[0],  12.54, 2);
  munit_assert_double_equal(a.data[1], -10.54, 2);
  munit_assert_double_equal(a.data[2], -10.54, 2);
  munit_assert_double_equal(a.data[3],  12.54, 2);
  munit_assert_int(b.rows, ==, 2);
  munit_assert_double_equal(b.data[0], -9.81, 2);
  munit_assert_double_equal(b.data[1], -9.81, 2);
  return MUNIT_OK;
}

static MunitResult test_nonzero_speed(const MunitParameter params[], void *data) {
  body_info_t info1 = body_info(1.0, inertia_cuboid(1.0, 1, 0.4, 0.2), vector(0, 0, 0), vector(0, 0, 0));
  body_info_t info2 = body_info(6e+24, inertia_cuboid(6e+24, 6370e+3, 6370e+3, 6370e+3), vector(0, 0, 0), vector(0, 0, 0));
  list_t body_infos = make_list(); append_body_info(&body_infos, info1); append_body_info(&body_infos, info2);
  state_t *s1 = state(vector(0, 0, 6370e+3 + 0.1), vector(0, 0, -3), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(0, 0, 0), vector(0, 0, 2), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  list_t states = make_list(); append_pointer(&states, s1); append_pointer(&states, s2);
  contact_t c = contact(0, 1, vector(0, 0, -1), vector(0, 0, 6370e+3));
  list_t contacts = make_list(); append_contact(&contacts, c);
  large_matrix_t a; large_vector_t b;
  frictionless_case(body_infos, states, contacts, &a, &b, 0.5);
  munit_assert_int(a.rows, ==, 1);
  munit_assert_int(a.cols, ==, 1);
  munit_assert_double_equal(a.data[0], 1.0, 6);
  munit_assert_int(b.rows, ==, 1);
  munit_assert_double_equal(b.data[0], -5.0, 6);
  return MUNIT_OK;
}

MunitTest test_linear_complementary[] = {
  {"/one_contact"  , test_one_contact  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/two_contacts" , test_two_contacts , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/nonzero_speed", test_nonzero_speed, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL            , NULL              , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
