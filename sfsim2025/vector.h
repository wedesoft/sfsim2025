#pragma once
#include <math.h>


typedef struct {
  double x;
  double y;
  double z;
} vector_t;

inline vector_t vector(double x, double y, double z)
{
  vector_t result;
  result.x = x;
  result.y = y;
  result.z = z;
  return result;
}

inline vector_t difference(vector_t a, vector_t b)
{
  vector_t result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
  result.z = a.z - b.z;
  return result;
}

inline vector_t cross_product(vector_t a, vector_t b)
{
  vector_t result;
  result.x = a.y * b.z - a.z * b.y;
  result.y = a.z * b.x - a.x * b.z;
  result.z = a.x * b.y - a.y * b.x;
  return result;
}

inline vector_t normalize(vector_t v)
{
  double norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  return vector(v.x / norm, v.y / norm, v.z / norm);
}
