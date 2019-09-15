#include "munit.h"

static const MunitSuite test_sfsim[] = {
  {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE}
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
  int result = munit_suite_main(&test_sfsim[0], (void *)NULL, argc, argv);
  return result;
}
