#include "sfsim2025/lemke.h"
#include "test_lemke.h"
#include "test_helper.h"


static MunitResult test_init_tableau(const MunitParameter params[], void *data) {
  large_matrix_t a = large_matrix(2, 2, (double []){2, 3, 5, 7});
  large_vector_t b = large_vector(2, (double []){11, 13});
  large_matrix_t t = initialize_tableau(a, b);
  munit_assert_int(t.rows, ==, 2);
  munit_assert_int(t.cols, ==, 6);
  munit_assert_double(t.data[0], ==,  1); munit_assert_double(t.data[ 1], ==,  0);
  munit_assert_double(t.data[6], ==,  0); munit_assert_double(t.data[ 7], ==,  1);
  munit_assert_double(t.data[2], ==, -2); munit_assert_double(t.data[ 3], ==, -3);
  munit_assert_double(t.data[8], ==, -5); munit_assert_double(t.data[ 9], ==, -7);
  munit_assert_double(t.data[4], ==, -1); munit_assert_double(t.data[10], ==, -1);
  munit_assert_double(t.data[5], ==, 11); munit_assert_double(t.data[11], ==, 13);
  return MUNIT_OK;
}

MunitTest test_lemke[] = {
  {"/init_tableau", test_init_tableau, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL           , NULL             , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL},
};
