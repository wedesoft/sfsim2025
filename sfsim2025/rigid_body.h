#pragma once
#include "list.h"


typedef struct {
  list_t *points;
  list_t *edges;
  list_t *faces;
} rigid_body_t;

rigid_body_t *make_rigid_body(void);
