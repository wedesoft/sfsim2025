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

static MunitResult test_pass_uniform(const MunitParameter params[], void *data) {
  friction_t f = uniform_friction(10);
  large_vector_t v = allocate_large_vector(2);
  v.data[0] = 3; v.data[1] = 4;
  large_vector_t l = limit_friction(f, 1, v);
  munit_assert_int(l.rows, ==, 2);
  munit_assert_double(l.data[0], ==, 3);
  munit_assert_double(l.data[1], ==, 4);
  return MUNIT_OK;
}

static MunitResult test_limit_uniform(const MunitParameter params[], void *data) {
  friction_t f = uniform_friction(5);
  large_vector_t v = allocate_large_vector(2);
  v.data[0] = 6; v.data[1] = 8;
  large_vector_t l = limit_friction(f, 1, v);
  munit_assert_int(l.rows, ==, 2);
  munit_assert_double(l.data[0], ==, 3);
  munit_assert_double(l.data[1], ==, 4);
  return MUNIT_OK;
}

static MunitResult test_use_contact_lambda(const MunitParameter params[], void *data) {
  friction_t f = uniform_friction(5);
  large_vector_t v = allocate_large_vector(2);
  v.data[0] = 6; v.data[1] = 8;
  large_vector_t l = limit_friction(f, 2, v);
  munit_assert_int(l.rows, ==, 2);
  munit_assert_double(l.data[0], ==, 6);
  munit_assert_double(l.data[1], ==, 8);
  return MUNIT_OK;
}

MunitTest test_friction[] = {
  {"/uniform_friction"    , test_uniform_friction    , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/anisotropic_friction", test_anisotropic_friction, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/pass_uniform"        , test_pass_uniform        , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/limit_uniform"       , test_limit_uniform       , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/use_contact_lambda"  , test_use_contact_lambda  , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                   , NULL                     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};
