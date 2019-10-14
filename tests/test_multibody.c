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
  munit_assert_double(s.data[ 90], ==, 1); munit_assert_double(s.data[ 91], ==, 0); munit_assert_double(s.data[ 92], ==, 0);
  munit_assert_double(s.data[102], ==, 0); munit_assert_double(s.data[103], ==, 1); munit_assert_double(s.data[104], ==, 0);
  munit_assert_double(s.data[114], ==, 0); munit_assert_double(s.data[115], ==, 0); munit_assert_double(s.data[116], ==, 1);
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

MunitTest test_multibody[] = {
  {"/state_adapter_shape1"     , test_state_adapter_shape1     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/state_adapter_shape2"     , test_state_adapter_shape2     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/state_adapter_identity"   , test_state_adapter_identity   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/state_adapter_identity2"  , test_state_adapter_identity2  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/state_adapter_orientation", test_state_adapter_orientation, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                        , NULL                          , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
