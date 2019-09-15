#pragma once
#include "list.h"
#include "point.h"
#include "face.h"


typedef struct {
  list_t *points;
  list_t *edges;
  list_t *faces;
} rigid_body_t;

rigid_body_t *make_rigid_body(void);

void add_point(rigid_body_t *body, point_t point);

void add_face(rigid_body_t *body, face_t face);
