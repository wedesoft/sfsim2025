#include "sfsim2025/friction.h"
#include "test_friction.h"
#include "test_helper.h"


static MunitResult test_uniform_friction(const MunitParameter params[], void *data) {
  friction_t f = uniform_friction(0.4);
  munit_assert_int(f.type, ==, UNIFORM_FRICTION);
  munit_assert_double(f.uniform, ==, 0.4);
  return MUNIT_OK;
}

static MunitResult test_anisotropic_friction(const MunitParameter params[], void *data) {
  friction_t f = anisotropic_friction(0.25, 0.5);
  munit_assert_int(f.type, ==, ANISOTROPIC_FRICTION);
  munit_assert_double(f.anisotropic.value1, ==, 0.25);
  munit_assert_double(f.anisotropic.value2, ==, 0.5);
  return MUNIT_OK;
}

MunitTest test_friction[] = {
  {"/uniform_friction"    , test_uniform_friction    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/anisotropic_friction", test_anisotropic_friction, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                   , NULL                     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
