#pragma once


typedef struct {
  float x;
  float y;
  float z;
} vector_t;

inline vector_t vector(float x, float y, float z)
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

vector_t normalize(vector_t v);
