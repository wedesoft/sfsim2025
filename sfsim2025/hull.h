#pragma once
#include <stdbool.h>
#include "list.h"
#include "vector.h"
#include "face.h"
#include "plane.h"
#include "quaternion.h"
#include "state.h"


typedef struct {
  list_t points;
  list_t edges;
  list_t faces;
} hull_t;

hull_t *make_hull(void);

void add_point(hull_t *body, vector_t point);

void add_face(hull_t *body, face_t face);

// Get coordinates of first point of edge.
static inline vector_t edge_tail(hull_t *body, edge_t edge) {
  return get_vector(body->points)[edge.a];
}

// Get coordinates of second point of edge.
static inline vector_t edge_head(hull_t *body, edge_t edge) {
  return get_vector(body->points)[edge.b];
}

// Get edge vector.
static inline vector_t edge_vector(hull_t *body, edge_t edge) {
  return vector_subtract(edge_head(body, edge), edge_tail(body, edge));
}

vector_t face_normal(hull_t *body, face_t face);

plane_t face_plane(hull_t *body, face_t face);

double smallest_distance(plane_t plane, hull_t *body, int *index);

double largest_distance(plane_t plane, hull_t *body, int *index);

double best_face(hull_t *body, hull_t *other, int *face_index, int *point_index);

bool edge_planes(hull_t *body, edge_t edge1, hull_t *other, edge_t edge2, plane_t *p1, plane_t *p2);

double best_edge_pair(hull_t *body, hull_t *other, int *edge1_index, int *edge2_index);

list_t penetration_candidates(plane_t p, hull_t *body);

plane_t separating_plane(hull_t *body, hull_t *other, double *distance);

list_t contact_points(hull_t *body, hull_t *other, double *distance, vector_t *normal);

list_t contacts(int i, int j, hull_t *transformed_body, hull_t *transformed_other, double restitution,
                double friction, state_t *state1, state_t *state2);

hull_t *transform_body(hull_t *body, quaternion_t q, vector_t t);

hull_t *make_cube(double w, double h, double d);
