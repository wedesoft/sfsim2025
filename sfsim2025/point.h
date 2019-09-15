#pragma once


typedef struct {
  float x;
  float y;
  float z;
} point_t;

inline point_t point(float x, float y, float z)
{
  point_t result;
  result.x = x;
  result.y = y;
  result.z = z;
  return result;
}
