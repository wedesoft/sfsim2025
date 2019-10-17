#include "sfsim2025/multibody.h"
#include "test_multibody.h"
#include "test_helper.h"


static MunitResult test_state_adapter_shape1(const MunitParameter params[], void *data) {
  quaternion_t q = quaternion(1, 2, 3, 4);
  large_matrix_t s = state_adapter(1, &q);
  munit_assert_int(s.rows, ==, 7);
  munit_assert_int(s.cols, ==, 6);
  return MUNIT_OK;
}

static MunitResult test_state_adapter_shape2(const MunitParameter params[], void *data) {
  quaternion_t q[] = {quaternion(1, 2, 3, 4), quaternion(5, 6, 7, 8)};
  large_matrix_t s = state_adapter(2, q);
  munit_assert_int(s.rows, ==, 14);
  munit_assert_int(s.cols, ==, 12);
  return MUNIT_OK;
}

static MunitResult test_state_adapter_identity(const MunitParameter params[], void *data) {
  quaternion_t q = quaternion(1, 2, 3, 4);
  large_matrix_t s = state_adapter(1, &q);
  munit_assert_double(s.data[ 0], ==, 1); munit_assert_double(s.data[ 1], ==, 0); munit_assert_double(s.data[ 2], ==, 0);
  munit_assert_double(s.data[ 6], ==, 0); munit_assert_double(s.data[ 7], ==, 1); munit_assert_double(s.data[ 8], ==, 0);
  munit_assert_double(s.data[12], ==, 0); munit_assert_double(s.data[13], ==, 0); munit_assert_double(s.data[14], ==, 1);
  return MUNIT_OK;
}

static MunitResult test_state_adapter_identity2(const MunitParameter params[], void *data) {
  quaternion_t q[] = {quaternion(1, 2, 3, 4), quaternion(5, 6, 7, 8)};
  large_matrix_t s = state_adapter(2, q);
  munit_assert_double(s.data[ 90], ==, 1);
  return MUNIT_OK;
}

static MunitResult test_state_adapter_orientation(const MunitParameter params[], void *data) {
  quaternion_t q = quaternion(1, 2, 3, 4);
  large_matrix_t s = state_adapter(1, &q);
  munit_assert_double(s.data[21], ==, -1.0); munit_assert_double(s.data[22], ==, -1.5); munit_assert_double(s.data[23], ==, -2.0);
  munit_assert_double(s.data[27], ==,  0.5); munit_assert_double(s.data[28], ==,  2.0); munit_assert_double(s.data[29], ==, -1.5);
  munit_assert_double(s.data[33], ==, -2.0); munit_assert_double(s.data[34], ==,  0.5); munit_assert_double(s.data[35], ==,  1.0);
  munit_assert_double(s.data[39], ==,  1.5); munit_assert_double(s.data[40], ==, -1.0); munit_assert_double(s.data[41], ==,  0.5);
  return MUNIT_OK;
}

static MunitResult test_state_adapter_orientation2(const MunitParameter params[], void *data) {
  quaternion_t q[] = {quaternion(1, 2, 3, 4), quaternion(5, 6, 7, 8)};
  large_matrix_t s = state_adapter(2, q);
  munit_assert_double(s.data[129], ==, -3.0);
  return MUNIT_OK;
}

static MunitResult test_mass_shape(const MunitParameter params[], void *data) {
  double mass = 10.0; matrix_t inertia = diagonal(2, 3, 5);
  large_matrix_t m = generalized_mass(1, &mass, &inertia);
  munit_assert_int(m.rows, ==, 6);
  munit_assert_int(m.cols, ==, 6);
  return MUNIT_OK;
}

static MunitResult test_mass_shape2(const MunitParameter params[], void *data) {
  double mass[] = {10.0, 20.0}; matrix_t inertia[] = {diagonal(2, 3, 5), diagonal(3, 5, 7)};
  large_matrix_t m = generalized_mass(2, mass, inertia);
  munit_assert_int(m.rows, ==, 12);
  munit_assert_int(m.cols, ==, 12);
  return MUNIT_OK;
}

static MunitResult test_mass_diagonal(const MunitParameter params[], void *data) {
  double mass = 10.0; matrix_t inertia = diagonal(2, 3, 5);
  large_matrix_t m = generalized_mass(1, &mass, &inertia);
  munit_assert_double(m.data[ 0], ==, 10); munit_assert_double(m.data[ 1], ==,  0); munit_assert_double(m.data[ 2], ==,  0);
  munit_assert_double(m.data[ 6], ==,  0); munit_assert_double(m.data[ 7], ==, 10); munit_assert_double(m.data[ 8], ==,  0);
  munit_assert_double(m.data[12], ==,  0); munit_assert_double(m.data[13], ==,  0); munit_assert_double(m.data[14], ==, 10);
  return MUNIT_OK;
}

static MunitResult test_mass_diagonal2(const MunitParameter params[], void *data) {
  double mass[] = {10.0, 20.0}; matrix_t inertia[] = {diagonal(2, 3, 5), diagonal(3, 5, 7)};
  large_matrix_t m = generalized_mass(2, mass, inertia);
  munit_assert_double(m.data[78], ==, 20);
  return MUNIT_OK;
}

static MunitResult test_mass_inertia(const MunitParameter params[], void *data) {
  double mass = 10.0; matrix_t inertia = diagonal(2, 3, 5);
  large_matrix_t m = generalized_mass(1, &mass, &inertia);
  munit_assert_double(m.data[21], ==, 2); munit_assert_double(m.data[22], ==, 0); munit_assert_double(m.data[23], ==, 0);
  munit_assert_double(m.data[27], ==, 0); munit_assert_double(m.data[28], ==, 3); munit_assert_double(m.data[29], ==, 0);
  munit_assert_double(m.data[33], ==, 0); munit_assert_double(m.data[34], ==, 0); munit_assert_double(m.data[35], ==, 5);
  return MUNIT_OK;
}

static MunitResult test_mass_inertia2(const MunitParameter params[], void *data) {
  double mass[] = {10.0, 20.0}; matrix_t inertia[] = {diagonal(2, 3, 5), diagonal(3, 5, 7)};
  large_matrix_t m = generalized_mass(2, mass, inertia);
  munit_assert_double(m.data[117], ==, 3);
  return MUNIT_OK;
}

static MunitResult test_contact_normals_shape(const MunitParameter params[], void *data) {
  vector_t normal = vector(1, 2, 3);
  large_matrix_t n = contact_normals(1, &normal);
  munit_assert_int(n.rows, ==, 3);
  munit_assert_int(n.cols, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_contact_normals_shape2(const MunitParameter params[], void *data) {
  vector_t normals[] = {vector(1, 2, 3), vector(4, 5, 6)};
  large_matrix_t n = contact_normals(2, normals);
  munit_assert_int(n.rows, ==, 6);
  munit_assert_int(n.cols, ==, 2);
  return MUNIT_OK;
}

static MunitResult test_normal_vectors(const MunitParameter params[], void *data) {
  vector_t normals[] = {vector(1, 2, 3), vector(4, 5, 6)};
  large_matrix_t n = contact_normals(2, normals);
  munit_assert_double(n.data[0], ==, 1); munit_assert_double(n.data[2], ==, 2); munit_assert_double(n.data[ 4], ==, 3);
  munit_assert_double(n.data[7], ==, 4); munit_assert_double(n.data[9], ==, 5); munit_assert_double(n.data[11], ==, 6);
  return MUNIT_OK;
}

static MunitResult test_conditions_shape(const MunitParameter params[], void *data) {
  list_t *contacts = make_list();
  append_contact(contacts, contact(0, 1, vector(0, 0, 1), vector(0, 0, 2)));
  list_t *bodies = make_list();
  append_pointer(bodies, make_rigid_body(vector(0, 0, 0)));
  append_pointer(bodies, make_rigid_body(vector(0, 0, 5)));
  large_matrix_t j = contact_conditions(contacts, bodies);
  munit_assert_int(j.rows, ==, 12);
  munit_assert_int(j.cols, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_conditions_content(const MunitParameter params[], void *data) {
  list_t *contacts = make_list();
  append_contact(contacts, contact(0, 1, vector(0, 0, 1), vector(0, 0, 2)));
  list_t *bodies = make_list();
  append_pointer(bodies, make_rigid_body(vector(0, 0, 0)));
  append_pointer(bodies, make_rigid_body(vector(0, 0, 5)));
  large_matrix_t j = contact_conditions(contacts, bodies);
  // negative identity matrix
  munit_assert_double(j.data[ 0], ==, -1); munit_assert_double(j.data[ 1], ==,  0); munit_assert_double(j.data[ 2], ==,  0);
  munit_assert_double(j.data[ 3], ==,  0); munit_assert_double(j.data[ 4], ==, -1); munit_assert_double(j.data[ 5], ==,  0);
  munit_assert_double(j.data[ 6], ==,  0); munit_assert_double(j.data[ 7], ==,  0); munit_assert_double(j.data[ 8], ==, -1);
  // negative skew-symmetric matrix
  munit_assert_double(j.data[ 9], ==,  0); munit_assert_double(j.data[10], ==,  2); munit_assert_double(j.data[11], ==,  0);
  munit_assert_double(j.data[12], ==, -2); munit_assert_double(j.data[13], ==,  0); munit_assert_double(j.data[14], ==,  0);
  munit_assert_double(j.data[15], ==,  0); munit_assert_double(j.data[16], ==,  0); munit_assert_double(j.data[17], ==,  0);
  // positive identity matrix
  munit_assert_double(j.data[18], ==,  1); munit_assert_double(j.data[19], ==,  0); munit_assert_double(j.data[20], ==,  0);
  munit_assert_double(j.data[21], ==,  0); munit_assert_double(j.data[22], ==,  1); munit_assert_double(j.data[23], ==,  0);
  munit_assert_double(j.data[24], ==,  0); munit_assert_double(j.data[25], ==,  0); munit_assert_double(j.data[26], ==,  1);
  // positive skew-symmetric matrix
  munit_assert_double(j.data[27], ==,  0); munit_assert_double(j.data[28], ==,  3); munit_assert_double(j.data[29], ==,  0);
  munit_assert_double(j.data[30], ==, -3); munit_assert_double(j.data[31], ==,  0); munit_assert_double(j.data[32], ==,  0);
  munit_assert_double(j.data[33], ==,  0); munit_assert_double(j.data[34], ==,  0); munit_assert_double(j.data[35], ==,  0);

  return MUNIT_OK;
}

MunitTest test_multibody[] = {
  {"/state_adapter_shape1"      , test_state_adapter_shape1      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/state_adapter_shape2"      , test_state_adapter_shape2      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/state_adapter_identity"    , test_state_adapter_identity    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/state_adapter_identity2"   , test_state_adapter_identity2   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/state_adapter_orientation" , test_state_adapter_orientation , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/state_adapter_orientation2", test_state_adapter_orientation2, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/mass_shape"                , test_mass_shape                , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/mass_shape2"               , test_mass_shape2               , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/mass_diagonal"             , test_mass_diagonal             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/mass_diagonal2"            , test_mass_diagonal2            , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/mass_inertia"              , test_mass_inertia              , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/mass_inertia2"             , test_mass_inertia2             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/contact_normals_shape"     , test_contact_normals_shape     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/contact_normals_shape2"    , test_contact_normals_shape2    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/normal_vectors"            , test_normal_vectors            , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/conditions_shape"          , test_conditions_shape          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/conditions_content"        , test_conditions_content        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                         , NULL                           , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
