#include "sfsim2025/image.h"
#include "test_image.h"


static MunitResult test_shape(const MunitParameter params[], void *data) {
  return MUNIT_OK;
}

MunitTest test_image[] = {
  {"/shape", test_shape, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL    , NULL      , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
