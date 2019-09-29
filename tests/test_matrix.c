#include "sfsim2025/matrix.h"
#include "test_helper.h"
#include "test_matrix.h"


static MunitResult test_create(const MunitParameter params[], void *data) {
  matrix_t *result = matrix(1, 2, 3, 4, 5, 6, 7, 8, 9);
  munit_assert_double(*result[0][0], ==, 1); munit_assert_double(*result[0][1], ==, 2); munit_assert_double(*result[0][2], ==, 3);
  munit_assert_double(*result[1][0], ==, 4); munit_assert_double(*result[1][1], ==, 5); munit_assert_double(*result[1][2], ==, 6);
  munit_assert_double(*result[2][0], ==, 7); munit_assert_double(*result[2][1], ==, 8); munit_assert_double(*result[2][2], ==, 9);
  return MUNIT_OK;
}

static MunitResult test_cross_product_matrix(const MunitParameter params[], void *data) {
  matrix_t *result = cross_product_matrix(vector(1, 2, 3));
  munit_assert_double(*result[0][0], ==,  0); munit_assert_double(*result[0][1], ==, -3); munit_assert_double(*result[0][2], ==,  2);
  munit_assert_double(*result[1][0], ==,  3); munit_assert_double(*result[1][1], ==,  0); munit_assert_double(*result[1][2], ==, -1);
  munit_assert_double(*result[2][0], ==, -2); munit_assert_double(*result[2][1], ==,  1); munit_assert_double(*result[2][2], ==,  0);
  return MUNIT_OK;
}

MunitTest test_matrix[] = {
  {"/create"              , test_create              , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/cross_product_matrix", test_cross_product_matrix, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                   , NULL                     , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
