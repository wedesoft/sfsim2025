#include <gc.h>
#include "munit.h"
#include "test_cache.h"
#include "test_contact.h"
#include "test_elevation.h"
#include "test_force.h"
#include "test_friction.h"
#include "test_image.h"
#include "test_joint.h"
#include "test_large_matrix.h"
#include "test_large_vector.h"
#include "test_list.h"
#include "test_map.h"
#include "test_matrix.h"
#include "test_mechanics.h"
#include "test_plane.h"
#include "test_polygon.h"
#include "test_projection.h"
#include "test_quaternion.h"
#include "test_hull.h"
#include "test_state.h"
#include "test_vector.h"
#include "test_world.h"


static MunitSuite test_sfsim2025[] = {
  {"/list"                , test_list                , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/vector"              , test_vector              , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/matrix"              , test_matrix              , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/large_vector"        , test_large_vector        , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/large_matrix"        , test_large_matrix        , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/quaternion"          , test_quaternion          , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/plane"               , test_plane               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/polygon"             , test_polygon             , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/hull"                , test_hull                , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/state"               , test_state               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/mechanics"           , test_mechanics           , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/contact"             , test_contact             , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/joint"               , test_joint               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/world"               , test_world               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/force"               , test_force               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/friction"            , test_friction            , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/image"               , test_image               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/elevation"           , test_elevation           , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/projection"          , test_projection          , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/map"                 , test_map                 , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/cache"               , test_cache               , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {NULL                   , NULL                     , NULL, 0, MUNIT_SUITE_OPTION_NONE}
};

static const MunitSuite test_suite[] = {
  {"", NULL, test_sfsim2025, 1, MUNIT_SUITE_OPTION_NONE}
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  int result = munit_suite_main(&test_suite[0], (void *)NULL, argc, argv);
  return result;
}
