#pragma once


typedef struct {
  int a;
  int b;
  int c;
} face_t;

inline face_t face(int a, int b, int c) {
  face_t result;
  result.a = a;
  result.b = b;
  result.c = c;
  return result;
}
