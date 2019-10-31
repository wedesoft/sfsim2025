#include "sfsim2025/large_vector.h"
#include "test_large_vector.h"
#include "test_helper.h"


static MunitResult test_create(const MunitParameter params[], void *data) {
  large_vector_t vector = large_vector(3, (double []){2, 3, 5});
  munit_assert_int(vector.rows, ==, 3);
  munit_assert_double(vector.data[0], ==, 2);
  munit_assert_double(vector.data[1], ==, 3);
  munit_assert_double(vector.data[2], ==, 5);
  return MUNIT_OK;
}

static MunitResult test_add(const MunitParameter params[], void *data) {
  large_vector_t a = large_vector(2, (double []){2, 3});
  large_vector_t b = large_vector(2, (double []){5, 7});
  large_vector_t result = large_vector_add(a, b);
  munit_assert_int(result.rows, ==, 2);
  munit_assert_double(result.data[0], ==,  7);
  munit_assert_double(result.data[1], ==, 10);
  return MUNIT_OK;
}

static MunitResult test_scale(const MunitParameter params[], void *data) {
  large_vector_t a = large_vector(2, (double []){2, 3});
  large_vector_t result = large_vector_scale(a, 2);
  munit_assert_int(result.rows, ==, 2);
  munit_assert_double(result.data[0], ==, 4);
  munit_assert_double(result.data[1], ==, 6);
  return MUNIT_OK;
}

static MunitResult test_negative(const MunitParameter params[], void *data) {
  large_vector_t result = large_vector_negative(large_vector(3, (double []){2, -3, 5}));
  munit_assert_int(result.rows, ==, 3);
  munit_assert_double(result.data[0], ==, -2);
  munit_assert_double(result.data[1], ==,  3);
  munit_assert_double(result.data[2], ==, -5);
  return MUNIT_OK;
}

MunitTest test_large_vector[] = {
  {"/create"  , test_create  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add"     , test_add     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/scale"   , test_scale   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/negative", test_negative, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL       , NULL         , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
