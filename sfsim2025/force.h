#pragma once


typedef enum {GRAVITATION} force_type_t;

typedef struct {
  force_type_t force_type;
  int i;
  int j;
} force_t;

static inline force_t gravitation(int i, int j) {
  return (force_t){.force_type = GRAVITATION, .i = i, .j = j};
}
