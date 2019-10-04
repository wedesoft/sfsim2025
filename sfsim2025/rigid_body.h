#pragma once
#include <stdbool.h>
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

static inline vector_t edge_tail(rigid_body_t *body, edge_t edge) {
  return get_vector(body->points)[edge.a];
}

static inline vector_t edge_head(rigid_body_t *body, edge_t edge) {
  return get_vector(body->points)[edge.b];
}

static inline vector_t edge_vector(rigid_body_t *body, edge_t edge) {
  return vector_subtract(edge_head(body, edge), edge_tail(body, edge));
}

vector_t face_normal(rigid_body_t *body, face_t face);

plane_t face_plane(rigid_body_t *body, face_t face);

double smallest_distance(plane_t plane, rigid_body_t *body, int *index);

double largest_distance(plane_t plane, rigid_body_t *body, int *index);

double best_face(rigid_body_t *body, rigid_body_t *other, int *face_index, int *point_index);

bool edge_planes(rigid_body_t *body, edge_t edge1, rigid_body_t *other, edge_t edge2, plane_t *p1, plane_t *p2);

double best_edge_pair(rigid_body_t *body, rigid_body_t *other, int *edge1_index, int *edge2_index);

list_t *penetration_candidates(plane_t p, rigid_body_t *body);

plane_t separating_plane(rigid_body_t *body, rigid_body_t *other, double *distance);

list_t *contact_points(rigid_body_t *body, rigid_body_t *other, double *distance);
