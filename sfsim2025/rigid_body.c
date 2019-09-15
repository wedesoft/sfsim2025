#include <gc.h>
#include "rigid_body.h"


rigid_body_t *make_rigid_body(void)
{
  rigid_body_t *result = GC_MALLOC(sizeof(rigid_body_t));
  result->points = make_list();
  result->edges = make_list();
  result->faces = make_list();
  return result;
}
