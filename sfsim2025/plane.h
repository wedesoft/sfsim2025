#pragma once
#include "list.h"
#include "vector.h"


typedef struct {
  vector_t point;
  vector_t normal;
} plane_t;

inline plane_t plane(vector_t point, vector_t normal) {
  plane_t result = {.point = point, .normal = normal};
  return result;
}

inline double plane_distance(plane_t plane, vector_t point) {
  return inner_product(vector_subtract(point, plane.point), plane.normal);
}

// Return plane with inverted normal vector.
inline plane_t negative_plane(plane_t p) {
  vector_t normal = p.normal;
  return plane(p.point, vector_negative(normal));
}

list_t *plane_coordinates(plane_t plane, list_t *vectors);

list_t *plane_points(plane_t plane, list_t *coordinates);
