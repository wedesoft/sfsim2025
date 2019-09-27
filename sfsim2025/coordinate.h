#pragma once
#include <stdbool.h>


typedef struct {
  double u;
  double v;
} coordinate_t;

inline coordinate_t coordinate(double u, double v) {
  coordinate_t result;
  result.u = u;
  result.v = v;
  return result;
}

inline bool coordinate_eq(coordinate_t a, coordinate_t b) {
  return a.u == b.u && a.v == b.v;
}

inline coordinate_t coordinate_difference(coordinate_t a, coordinate_t b) {
  return coordinate(a.u - b.u, a.v - b.v);
}
