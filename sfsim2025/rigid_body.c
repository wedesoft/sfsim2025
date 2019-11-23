#include <assert.h>
#include <float.h>
#include <gc.h>
#include "polygon.h"
#include "rigid_body.h"


// Create an empty rigid body.
rigid_body_t *make_rigid_body(void) {
  rigid_body_t *result = GC_MALLOC(sizeof(rigid_body_t));
  result->points = make_list();
  result->edges = make_list();
  result->faces = make_list();
  return result;
}

// Add 3D point to a rigid body.
void add_point(rigid_body_t *body, vector_t point) {
  append_vector(&body->points, point);
}

// Add edge to a rigid body.
void add_edge(rigid_body_t *body, edge_t edge) {
  for (int i=0; i<body->edges.size; i++) {
    edge_t current = get_edge(body->edges)[i];
    if (edge_eq(current, edge)) return;
    if (edge_eq(current, swap_edge(edge))) return;
  };
  append_edge(&body->edges, edge);
}

// Add face and corresponding edges to a rigid body.
void add_face(rigid_body_t *body, face_t face) {
  append_face(&body->faces, face);
  add_edge(body, edge(face.a, face.b));
  add_edge(body, edge(face.b, face.c));
  add_edge(body, edge(face.c, face.a));
}

// Get normal vector of a body's face.
vector_t face_normal(rigid_body_t *body, face_t face) {
  vector_t a = get_vector(body->points)[face.a];
  vector_t b = get_vector(body->points)[face.b];
  vector_t c = get_vector(body->points)[face.c];
  vector_t u = vector_subtract(b, a);
  vector_t v = vector_subtract(c, a);
  return normalize(cross_product(u, v));
}

// Get face plane of a body's face.
plane_t face_plane(rigid_body_t *body, face_t face) {
  plane_t result;
  result.point = get_vector(body->points)[face.a];
  result.normal = face_normal(body, face);
  return result;
}

// Get point of body with smallest distance to specified plane.
double smallest_distance(plane_t plane, rigid_body_t *body, int *index) {
  double result = DBL_MAX;
  for (int i=0; i<body->points.size; i++) {
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
  for (int i=0; i<body->points.size; i++) {
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
  for (int i=0; i<body->faces.size; i++) {
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

#define NORM_EPS 1e-3
#define DISTANCE_EPS 1e-3

// Construct separating planes from a pair of edges.
bool edge_planes(rigid_body_t *body, edge_t edge1, rigid_body_t *other, edge_t edge2, plane_t *p1, plane_t *p2) {
  assert(p1);
  assert(p2);
  vector_t orthogonal = cross_product(edge_vector(body, edge1), edge_vector(other, edge2));
  if (norm(orthogonal) < NORM_EPS)
    return false;
  vector_t normal = normalize(orthogonal);
  p1->point = edge_tail(body, edge1);
  p2->point = edge_tail(other, edge2);
  p1->normal = normal;
  p2->normal = vector_negative(normal);
  if (largest_distance(*p1, body, NULL) <= DISTANCE_EPS && largest_distance(*p2, other, NULL) <= DISTANCE_EPS)
    return true;
  p1->normal = vector_negative(normal);
  p2->normal = normal;
  if (largest_distance(*p1, body, NULL) <= DISTANCE_EPS && largest_distance(*p2, other, NULL) <= DISTANCE_EPS)
    return true;
  return false;
}

// Get best separating plane constructed from two edges.
double best_edge_pair(rigid_body_t *body, rigid_body_t *other, int *edge1_index, int *edge2_index) {
  double result = -DBL_MAX;
  for (int j=0; j<body->edges.size; j++) {
    for (int i=0; i<other->edges.size; i++) {
      edge_t edge1 = get_edge(body->edges)[j];
      edge_t edge2 = get_edge(other->edges)[i];
      plane_t p1;
      plane_t p2;
      if (edge_planes(body, edge1, other, edge2, &p1, &p2)) {
        double distance = plane_distance(p1, p2.point);
        if (distance > result) {
          result = distance;
          if (edge1_index) *edge1_index = j;
          if (edge2_index) *edge2_index = i;
        };
      };
    };
  };
  return result;
}

#define PENETRATION_EPS 1e-3

// Get points which are on the separating plane
list_t penetration_candidates(plane_t p, rigid_body_t *body) {
  list_t result = make_list();
  for (int i=0; i<body->points.size; i++) {
    vector_t v = get_vector(body->points)[i];
    if (plane_distance(p, v) <= PENETRATION_EPS)
      append_vector(&result, v);
  };
  return result;
}

// Get best separating plane between two polyhedra.
plane_t separating_plane(rigid_body_t *body, rigid_body_t *other, double *distance) {
  assert(body->faces.size > 0);
  assert(other->faces.size > 0);
  int face1_index, point1_index;
  double separation1 = best_face(body, other, &face1_index, &point1_index);
  int face2_index, point2_index;
  double separation2 = best_face(other, body, &face2_index, &point2_index);
  int edge1_index, edge2_index;
  double separation3 = best_edge_pair(body, other, &edge1_index, &edge2_index);
  plane_t result;
  if (separation1 >= separation2 && separation1 >= separation3) {
    plane_t p = face_plane(body, get_face(body->faces)[face1_index]);
    result = plane(vector_add(p.point, vector_scale(p.normal, 0.5 * separation1)), p.normal);
    if (distance) *distance = separation1;
  } else if (separation2 >= separation1 && separation2 >= separation3) {
    plane_t p = face_plane(other, get_face(other->faces)[face2_index]);
    result = plane(vector_add(p.point, vector_scale(p.normal, 0.5 * separation2)), vector_negative(p.normal));
    if (distance) *distance = separation2;
  } else {
    edge_t edge1 = get_edge(body->edges)[edge1_index];
    edge_t edge2 = get_edge(other->edges)[edge2_index];
    plane_t p1, p2;
    edge_planes(body, edge1, other, edge2, &p1, &p2);
    result = plane(vector_add(p1.point, vector_scale(p1.normal, 0.5 * separation3)), p1.normal);
    if (distance) *distance = separation3;
  };
  return result;
}

// Get contact points, contact normal, and distance between two bodies.
list_t contact_points(rigid_body_t *body, rigid_body_t *other, double *distance, vector_t *normal) {
  list_t result;
  if (body->faces.size > 0 && other->faces.size > 0) {
    plane_t p = separating_plane(body, other, distance);
    if (normal) *normal = p.normal;
    list_t candidates1 = convex_hull(plane_coordinates(p, penetration_candidates(negative_plane(p), body)));
    list_t candidates2 = convex_hull(plane_coordinates(p, penetration_candidates(p, other)));
    result = plane_points(p, convex_hull(intersection(candidates1, candidates2)));
  } else
    result = make_list();
  return result;
}

// Get list of contacts for two objects.
list_t contacts(int i, int j, rigid_body_t *body, rigid_body_t *other) {
  double distance;
  vector_t normal;
  list_t points = contact_points(body, other, &distance, &normal);
  list_t result = make_list();
  for (int k=0; k<points.size; k++) {
    contact_t c = contact(i, j, normal, get_vector(points)[k], distance);
    append_contact(&result, c);
  };
  return result;
}

// Rotate and translate the points of a rigid body.
rigid_body_t *transform_body(rigid_body_t *body, quaternion_t q, vector_t t) {
  rigid_body_t *result = GC_MALLOC(sizeof(rigid_body_t));
  result->points = make_list();
  result->edges = body->edges;
  result->faces = body->faces;
  for (int i=0; i<body->points.size; i++) {
    vector_t p = get_vector(body->points)[i];
    append_vector(&result->points, transform_point(q, t, p));
  };
  return result;
}
