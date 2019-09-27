#include "sfsim2025/vector.h"
#include "test_vector.h"
#include "test_helper.h"


static MunitResult test_negative(const MunitParameter params[], void *data) {
  vector_t result = negative(vector(2, 3, 5));
  munit_assert_double(result.x, ==, -2.0);
  munit_assert_double(result.y, ==, -3.0);
  munit_assert_double(result.z, ==, -5.0);
  return MUNIT_OK;
}

static MunitResult test_sum(const MunitParameter params[], void *data) {
  vector_t result = vector_add(vector(7, 11, 13), vector(2, 3, 5));
  munit_assert_double(result.x, ==, 9);
  munit_assert_double(result.y, ==, 14);
  munit_assert_double(result.z, ==, 18);
  return MUNIT_OK;
}

static MunitResult test_scale(const MunitParameter params[], void *data) {
  vector_t result = vector_scale(vector(7, 11, 13), 2);
  munit_assert_double(result.x, ==, 14);
  munit_assert_double(result.y, ==, 22);
  munit_assert_double(result.z, ==, 26);
  return MUNIT_OK;
}

static MunitResult test_difference(const MunitParameter params[], void *data) {
  vector_t result = vector_subtract(vector(7, 11, 13), vector(2, 3, 5));
  munit_assert_double(result.x, ==, 5);
  munit_assert_double(result.y, ==, 8);
  munit_assert_double(result.z, ==, 8);
  return MUNIT_OK;
}

static MunitResult test_cross_product(const MunitParameter params[], void *data) {
  vector_t result = cross_product(vector(1, 0, 0), vector(0, 1, 0));
  munit_assert_double(result.x, ==, 0.0);
  munit_assert_double(result.y, ==, 0.0);
  munit_assert_double(result.z, ==, 1.0);
  return MUNIT_OK;
}

static MunitResult test_norm(const MunitParameter params[], void *data) {
  munit_assert_double(norm(vector(3, 4, 0)), ==, 5);
  return MUNIT_OK;
}

static MunitResult test_normalize(const MunitParameter params[], void *data) {
  vector_t result = normalize(vector(3, 4, 0));
  munit_assert_double(result.x, ==, 0.6);
  munit_assert_double(result.y, ==, 0.8);
  munit_assert_double(result.z, ==, 0.0);
  return MUNIT_OK;
}

static MunitResult test_inner_product(const MunitParameter params[], void *data) {
  munit_assert_double(inner_product(vector(2, 3, 5), vector(3, 5, 7)), ==, 56);
  return MUNIT_OK;
}

static MunitResult test_not_parallel1(const MunitParameter params[], void *data) {
  vector_t v = not_parallel(vector(2, -3, 1));
  munit_assert_double(v.x, ==, 0);
  munit_assert_double(v.y, ==, 0);
  munit_assert_double(v.z, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_not_parallel2(const MunitParameter params[], void *data) {
  vector_t v = not_parallel(vector(2, 1, -3));
  munit_assert_double(v.x, ==, 0);
  munit_assert_double(v.y, ==, 1);
  munit_assert_double(v.z, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_not_parallel3(const MunitParameter params[], void *data) {
  vector_t v = not_parallel(vector(1, 2, -3));
  munit_assert_double(v.x, ==, 1);
  munit_assert_double(v.y, ==, 0);
  munit_assert_double(v.z, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_not_parallel4(const MunitParameter params[], void *data) {
  vector_t v = not_parallel(vector(-3, 2, 1));
  munit_assert_double(v.x, ==, 0);
  munit_assert_double(v.y, ==, 0);
  munit_assert_double(v.z, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_orthogonal1(const MunitParameter params[], void *data) {
  vector_t v = orthogonal1(vector(1, 0, 0));
  munit_assert_double(v.x, ==, 0);
  munit_assert_double(v.y, ==, 1);
  munit_assert_double(v.z, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_orthogonal2(const MunitParameter params[], void *data) {
  vector_t v = orthogonal2(vector(1, 0, 0));
  munit_assert_double(v.x, ==, 0);
  munit_assert_double(v.y, ==, 0);
  munit_assert_double(v.z, ==, 1);
  return MUNIT_OK;
}

MunitTest test_vector[] = {
  {"/negative"     , test_negative     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/sum"          , test_sum          , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/difference"   , test_difference   , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale"        , test_scale        , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cross_product", test_cross_product, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/inner_product", test_inner_product, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/norm"         , test_norm         , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/normalize"    , test_normalize    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/not_parallel1", test_not_parallel1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/not_parallel2", test_not_parallel2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/not_parallel3", test_not_parallel3, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/not_parallel4", test_not_parallel4, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/orthogonal1"  , test_orthogonal1  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/orthogonal2"  , test_orthogonal2  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL            , NULL              , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
