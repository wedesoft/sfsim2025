#pragma once


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
