#pragma once
#include "vector.h"


typedef struct {
  vector_t point;
  vector_t normal;
} plane_t;

inline plane_t plane(vector_t point, vector_t normal) {
  plane_t result;
  result.point = point;
  result.normal = normal;
  return result;
}

inline double plane_distance(plane_t plane, vector_t point) {
  return inner_product(difference(point, plane.point), plane.normal);
}
