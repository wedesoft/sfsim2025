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


MunitTest test_large_vector[] = {
  {"/create" , test_create , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL      , NULL        , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
