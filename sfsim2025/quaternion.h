#pragma once
#include "vector.h"


typedef struct {
  double a;
  double b;
  double c;
  double d;
} quaternion_t;

// Initialize quaternion.
inline quaternion_t quaternion(double a, double b, double c, double d) {
  return (quaternion_t){.a = a, .b = b, .c = c, .d = d};
}

inline quaternion_t vector_to_quaternion(vector_t v) {
  return (quaternion_t){.a = 0, .b = v.x, .c = v.y, .d = v.z};
}
