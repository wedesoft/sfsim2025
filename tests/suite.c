#include <gc.h>
#include "munit.h"
#include "test_contact.h"
#include "test_joint.h"
#include "test_large_matrix.h"
#include "test_lemke.h"
#include "test_large_vector.h"
#include "test_linear_complementary.h"
#include "test_list.h"
#include "test_matrix.h"
#include "test_matrix43.h"
#include "test_mechanics.h"
#include "test_multibody.h"
#include "test_plane.h"
#include "test_polygon.h"
#include "test_quaternion.h"
#include "test_rigid_body.h"
#include "test_state.h"
#include "test_vector.h"


static MunitSuite test_sfsim[] = {
  {"/list"                , test_list                , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/vector"              , test_vector              , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/matrix"              , test_matrix              , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/matrix43"            , test_matrix43            , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/large_vector"        , test_large_vector        , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/large_matrix"        , test_large_matrix        , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/quaternion"          , test_quaternion          , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/plane"               , test_plane               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/polygon"             , test_polygon             , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/rigid_body"          , test_rigid_body          , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/state"               , test_state               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/mechanics"           , test_mechanics           , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/contact"             , test_contact             , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/multibody"           , test_multibody           , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/linear_complementary", test_linear_complementary, NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/lemke"               , test_lemke               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/joint"               , test_joint               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {NULL                   , NULL                     , NULL, 0, MUNIT_SUITE_OPTION_NONE}
};

static const MunitSuite test_suite[] = {
  {"", NULL, test_sfsim, 1, MUNIT_SUITE_OPTION_NONE}
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  int result = munit_suite_main(&test_suite[0], (void *)NULL, argc, argv);
  return result;
}
