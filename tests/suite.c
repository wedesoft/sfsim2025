#include <gc.h>
#include "munit.h"
#include "test_list.h"
#include "test_rigid_body.h"

static MunitSuite test_sfsim[] = {
  {"/list"      , test_list      , NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {"/rigid_body", test_rigid_body, NULL, 1, MUNIT_SUITE_OPTION_NONE},
  {NULL         , NULL           , NULL, 0, MUNIT_SUITE_OPTION_NONE}
};

static const MunitSuite test_suite[] = {
  "", NULL, test_sfsim, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
  GC_INIT();
  int result = munit_suite_main(&test_suite[0], (void *)NULL, argc, argv);
  GC_gcollect();
  return result;
}
