#include <float.h>
#include <gc.h>
#include "rigid_body.h"


rigid_body_t *make_rigid_body(void) {
  rigid_body_t *result = GC_MALLOC(sizeof(rigid_body_t));
  result->points = make_list();
  result->edges = make_list();
  result->faces = make_list();
  return result;
}

void add_point(rigid_body_t *body, vector_t point) {
  append_vector(body->points, point);
}

void add_edge(rigid_body_t *body, edge_t edge) {
  for (int i=0; i<body->edges->size; i++) {
    edge_t current = get_edge(body->edges)[i];
    if (current.a == edge.a && current.b == edge.b) return;
    if (current.a == edge.b && current.b == edge.a) return;
  };
  append_edge(body->edges, edge);
}

void add_face(rigid_body_t *body, face_t face) {
  append_face(body->faces, face);
  add_edge(body, edge(face.a, face.b));
  add_edge(body, edge(face.b, face.c));
  add_edge(body, edge(face.c, face.a));
}

vector_t face_normal(rigid_body_t *body, face_t face) {
  vector_t a = get_vector(body->points)[face.a];
  vector_t b = get_vector(body->points)[face.b];
  vector_t c = get_vector(body->points)[face.c];
  vector_t u = difference(b, a);
  vector_t v = difference(c, a);
  return normalize(cross_product(u, v));
}

plane_t face_plane(rigid_body_t *body, face_t face) {
  plane_t result;
  result.point = get_vector(body->points)[face.a];
  result.normal = face_normal(body, face);
  return result;
}

// Get point of body with smallest distance to specified plane.
double smallest_distance(plane_t plane, rigid_body_t *body, int *index) {
  double result = DBL_MAX;
  for (int i=0; i<body->points->size; i++) {
    double distance = plane_distance(plane, get_vector(body->points)[i]);
    if (distance < result) {
      result = distance;
      if (index) *index = i;
    };
  };
  return result;
}

// Get point of body with largest distance to specified plane.
double largest_distance(plane_t plane, rigid_body_t *body, int *index) {
  double result = -DBL_MAX;
  for (int i=0; i<body->points->size; i++) {
    double distance = plane_distance(plane, get_vector(body->points)[i]);
    if (distance > result) {
      result = distance;
      if (index) *index = i;
    };
  };
  return result;
}

// Get face-point combination from two bodies with greatest separation.
double best_face(rigid_body_t * body, rigid_body_t *other, int *face_index, int *point_index) {
  double result = -DBL_MAX;
  for (int i=0; i<body->faces->size; i++) {
    int index;
    double distance = smallest_distance(face_plane(body, get_face(body->faces)[i]), other, &index);
    if (distance > result) {
      result = distance;
      if (face_index) *face_index = i;
      if (point_index) *point_index = index;
    };
  };
  return result;
}
