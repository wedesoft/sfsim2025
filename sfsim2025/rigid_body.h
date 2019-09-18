#pragma once
#include "list.h"
#include "vector.h"
#include "face.h"
#include "plane.h"


typedef struct {
  list_t *points;
  list_t *edges;
  list_t *faces;
} rigid_body_t;

rigid_body_t *make_rigid_body(void);

void add_point(rigid_body_t *body, vector_t point);

void add_face(rigid_body_t *body, face_t face);

vector_t face_normal(rigid_body_t *body, face_t face);

plane_t face_plane(rigid_body_t *body, face_t face);

double smallest_distance(plane_t plane, rigid_body_t *body, int *index);

double best_face(rigid_body_t * body, rigid_body_t *other, int *face_index, int *point_index);
