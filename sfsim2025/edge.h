#pragma once


typedef struct {
  int a;
  int b;
} edge_t;

static inline edge_t edge(int a, int b) {
  edge_t result;
  result.a = a;
  result.b = b;
  return result;
}

static inline edge_t swap_edge(edge_t e)
{
  return edge(e.b, e.a);
}

static inline bool edge_eq(edge_t edge1, edge_t edge2) {
  return edge1.a == edge2.a && edge1.b == edge2.b;
}
