#include <assert.h>
#include <tgmath.h>
#include <stdbool.h>
#include <float.h>
#include "coordinate.h"
#include "polygon.h"


// Cross-product z-component for 2D points.
static inline double cross_product_coords(coordinate_t r, coordinate_t s) {
  return r.u * s.v - r.v * s.u;
}

// Compute z-component of cross product of a minus b and a minus c.
static inline double cross_product_abc(coordinate_t a, coordinate_t b, coordinate_t c) {
  coordinate_t r = coordinate_difference(a, b);
  coordinate_t s = coordinate_difference(a, c);
  return cross_product_coords(r, s);
}

// Check whether point c is further from a than point b.
static inline bool further(coordinate_t a, coordinate_t b, coordinate_t c) {
  double v1 = a.v - b.v;
  double v2 = a.v - c.v;
  double u1 = a.u - b.u;
  double u2 = a.u - c.u;
  return v2 * v2 + u2 * u2 > v1 * v1 + u1 * u1;
}

static list_t remove_duplicates(list_t polygon, double epsilon) {
  list_t result = make_list();
  for (int i=0; i<polygon.size; i++) {
    coordinate_t current = get_coordinate(polygon)[i];
    bool duplicate = false;
    for (int j=i+1; j<polygon.size; j++) {
      coordinate_t other = get_coordinate(polygon)[j];
      if (fabs(current.u - other.u) < epsilon && fabs(current.v - other.v) < epsilon) {
        duplicate = true;
        break;
      };
    };
    if (!duplicate)
      append_coordinate(&result, current);
  };
  return result;
}

// Compute convex hull of 2D point cloud and return resulting polygon in counter clockwise order.
// https://www.tutorialspoint.com/Jarvis-March-Algorithm
list_t convex_hull(list_t polygon) {
  polygon = remove_duplicates(polygon, 1e-4);
  list_t result = make_list();
  if (polygon.size == 0) return result;
  coordinate_t start = coordinate(DBL_MAX, DBL_MAX);
  for (int i=0; i<polygon.size; i++) {
    coordinate_t current = get_coordinate(polygon)[i];
    if (current.u < start.u || (current.u == start.u && current.v > start.v))
      start = current;  // Find leftmost (and then bottommost) start point.
  };
  coordinate_t current = start;
  append_coordinate(&result, current);
  while (true) {
    assert(result.size <= polygon.size);
    coordinate_t next = current;
    for (int i=0; i<polygon.size; i++) {
      coordinate_t point = get_coordinate(polygon)[i];
      if (coordinate_eq(point, current)) continue;  // Skip current point.
      if (coordinate_eq(next, current))
        next = point;  // Pick first point.
      else {
        double prod = cross_product_abc(current, next, point);
        if (prod < 0)
          next = point;  // Prefer point on the right side of the current edge.
        else if (prod == 0 && further(current, next, point))
          next = point;  // Skip collinear points.
      };
    };
    if (coordinate_eq(next, start)) break;  // Finish when getting back to the start point.
    append_coordinate(&result, next);
    current = next;
  };
  return result;
}

// Decide which side of an edge the point is on.
static inline bool side(coordinate_t a, coordinate_t b, coordinate_t p) {
  coordinate_t edge = coordinate_difference(b, a);
  coordinate_t vector = coordinate_difference(p, a);
  return edge.v * vector.u - edge.u * vector.v > 0;
}

// Check whether point is inside polygon (must be specified in counter clockwise order!).
bool inside(list_t points, coordinate_t point) {
  if (points.size <= 1)
    return false;
  for (int i=0; i<points.size; i++) {
    int j = i < points.size - 1 ? i + 1 : 0;
    if (side(get_coordinate(points)[i], get_coordinate(points)[j], point))
      return false;
  };
  return true;
}

// Return pointcloud defining intersection of two convex counter clockwise polygons.
// https://www.swtestacademy.com/intersection-convex-polygons-algorithm/
list_t intersection(list_t a, list_t b) {
  list_t result = make_list();
  for (int i=0; i<a.size; i++) {
    coordinate_t p = get_coordinate(a)[i];
    if (inside(b, p))  // Find points of a which are inside b.
      append_coordinate(&result, p);
  };
  for (int i=0; i<b.size; i++) {
    coordinate_t p = get_coordinate(b)[i];
    if (inside(a, p))  // Find points of b which are inside a.
      append_coordinate(&result, p);
  };
  for (int i=0; i<a.size; i++) {
    int i2 = i < a.size - 1 ? i + 1 : 0;
    coordinate_t p = get_coordinate(a)[i];
    coordinate_t p2 = get_coordinate(a)[i2];
    coordinate_t r = coordinate_difference(p2, p);
    for (int j=0; j<b.size; j++) {
      int j2 = j < b.size - 1 ? j + 1 : 0;
      coordinate_t q = get_coordinate(b)[j];
      coordinate_t q2 = get_coordinate(b)[j2];
      coordinate_t s = coordinate_difference(q2, q);
      if (side(p, p2, q) != side(p, p2, q2) && side(q, q2, p) != side(q, q2, p2)) {  // Find intersection of edges of a and b.
        // https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
        double t = cross_product_coords(coordinate_difference(q, p), s) / cross_product_coords(r, s);
        append_coordinate(&result, coordinate(p.u + t * r.u, p.v + t * r.v));
      };
    };
  };
  return result;
}
