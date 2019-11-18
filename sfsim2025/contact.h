#pragma once
#include "vector.h"


typedef struct {
  int i;
  int j;
  vector_t normal;
  vector_t point;
  double distance;
} contact_t;

// Initialize contact.
static inline contact_t contact(int i, int j, vector_t normal, vector_t point, double distance) {
  return (contact_t){.i = i, .j = j, .normal = normal, .point = point, .distance = distance};
}
