#pragma once


typedef struct {
  int a;
  int b;
} edge_t;

inline edge_t edge(int a, int b)
{
  edge_t result;
  result.a = a;
  result.b = b;
  return result;
}
