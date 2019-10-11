#include "sfsim2025/large_matrix.h"
#include "test_large_matrix.h"
#include "test_helper.h"


static MunitResult test_create(const MunitParameter params[], void *data) {
  large_matrix_t matrix = large_matrix(2, 3, (double []){2, 3, 5, 3, 5, 7});
  munit_assert_int(matrix.rows, ==, 2); munit_assert_int(matrix.cols, ==, 3);
  munit_assert_double(matrix.data[0], ==, 2); munit_assert_double(matrix.data[1], ==, 3); munit_assert_double(matrix.data[2], ==, 5);
  munit_assert_double(matrix.data[3], ==, 3); munit_assert_double(matrix.data[4], ==, 5); munit_assert_double(matrix.data[5], ==, 7);
  return MUNIT_OK;
}

static MunitResult test_dot(const MunitParameter params[], void *data) {
  large_matrix_t a = large_matrix(2, 3, (double []){2, 3, 5, 3, 5, 7});
  large_matrix_t b = large_matrix(3, 1, (double []){2, 3, 5});
  large_matrix_t result = large_matrix_dot(a, b);
  munit_assert_int(result.rows, ==, 2); munit_assert_int(result.cols, ==, 1);
  munit_assert_double(result.data[0], ==, 38);
  munit_assert_double(result.data[1], ==, 56);
  return MUNIT_OK;
}

static MunitResult test_transpose(const MunitParameter params[], void *data) {
  large_matrix_t result = large_transpose(large_matrix(2, 3, (double []){2, 3, 5, 3, 5, 7}));
  munit_assert_int(result.rows, ==, 3); munit_assert_int(result.cols, ==, 2);
  munit_assert_double(result.data[0], ==, 2); munit_assert_double(result.data[1], ==, 3);
  munit_assert_double(result.data[2], ==, 3); munit_assert_double(result.data[3], ==, 5);
  munit_assert_double(result.data[4], ==, 5); munit_assert_double(result.data[5], ==, 7);
  return MUNIT_OK;
}

static MunitResult test_inverse(const MunitParameter params[], void *data) {
  large_matrix_t a = large_matrix(2, 2, (double []){2, 3, 5, 7});
  large_matrix_t b = large_inverse(a);
  large_matrix_t result = large_matrix_dot(a, b);
  munit_assert_double_equal(result.data[0], 1, 6); munit_assert_double_equal(result.data[1], 0, 6);
  munit_assert_double_equal(result.data[2], 0, 6); munit_assert_double_equal(result.data[3], 1, 6);
  return MUNIT_OK;
}

MunitTest test_large_matrix[] = {
  {"/create"   , test_create   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/dot"      , test_dot      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/transpose", test_transpose, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/inverse"  , test_inverse  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL        , NULL          , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
