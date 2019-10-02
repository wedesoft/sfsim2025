#pragma once
#include "list.h"
#include "vector.h"


typedef struct {
  vector_t point;
  vector_t normal;
} plane_t;

inline plane_t plane(vector_t point, vector_t normal) {
  return (plane_t){.point = point, .normal = normal};
}

inline double plane_distance(plane_t plane, vector_t point) {
  return inner_product(vector_subtract(point, plane.point), plane.normal);
}

// Return plane with inverted normal vector.
inline plane_t negative_plane(plane_t p) {
  return plane(p.point, vector_negative(p.normal));
}

list_t *plane_coordinates(plane_t plane, list_t *vectors);

list_t *plane_points(plane_t plane, list_t *coordinates);
