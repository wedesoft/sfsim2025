#pragma once


typedef struct {
  double a;
  double b;
  double c;
  double d;
} quaternion_t;

// Initialize quaternion.
inline quaternion_t quaternion(double a, double b, double c, double d) {
  quaternion_t result = {.a = a, .b = b, .c = c, .d = d};
  return result;
}
