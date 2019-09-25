#include <stdbool.h>
#include <float.h>
#include "coordinate.h"
#include "hull.h"


// Compute z-component of cross product of a minus b and a minus c.
static double cross_product_z(coordinate_t a, coordinate_t b, coordinate_t c)
{
  double v1 = a.v - b.v;
  double v2 = a.v - c.v;
  double u1 = a.u - b.u;
  double u2 = a.u - c.u;
  return v2 * u1 - v1 * u2;
}

// Compute convex hull of 2D polygon.
// https://www.tutorialspoint.com/Jarvis-March-Algorithm
list_t *convex_hull(list_t *polygon) {
  list_t *result = make_list();
  coordinate_t start = coordinate(DBL_MAX, DBL_MAX);
  for (int i=0; i<polygon->size; i++) {
    coordinate_t current = get_coordinate(polygon)[i];
    if (current.u < start.u || (current.u == start.u && current.v < start.v))
      start = current;
  };
  coordinate_t current = start;
  append_coordinate(result, current);
  while (true) {
    coordinate_t next = current;
    for (int i=0; i<polygon->size; i++) {
      coordinate_t point = get_coordinate(polygon)[i];
      if (point.u == current.u && point.v == current.v) continue;
      if (next.u == current.u && next.v == current.v)
        next = point;
      else if (cross_product_z(current, next, point) > 0)
        next = point;
    };
    if (next.u == start.u && next.v == start.v)
      break;
    append_coordinate(result, next);
    current = next;
  };
  return result;
}
