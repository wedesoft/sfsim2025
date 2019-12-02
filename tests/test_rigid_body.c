#include "sfsim2025/rigid_body.h"
#include "test_rigid_body.h"
#include "test_helper.h"


static rigid_body_t *make_tetrahedron(double scale, double z) {
  //      3---2    z^  ^
  //     / \  |     | /y
  //    /   \ |  ---+---->
  //   /     \|    /|   x
  // 0--------1
  rigid_body_t *result = make_rigid_body();
  add_point(result, vector(0, 0, z));
  add_point(result, vector(scale, 0, z));
  add_point(result, vector(0, scale, z));
  add_point(result, vector(0, 0, z + scale));
  add_face(result, face(0, 2, 1));
  add_face(result, face(0, 3, 2));
  add_face(result, face(0, 1, 3));
  add_face(result, face(1, 2, 3));
  return result;
}

static void add_cube_faces(rigid_body_t *body) {
  add_face(body, face(0, 1, 3));
  add_face(body, face(0, 3, 2));
  add_face(body, face(5, 4, 7));
  add_face(body, face(5, 6, 7));
  add_face(body, face(4, 0, 2));
  add_face(body, face(4, 2, 6));
  add_face(body, face(1, 5, 7));
  add_face(body, face(1, 7, 3));
  add_face(body, face(0, 5, 1));
  add_face(body, face(0, 4, 5));
  add_face(body, face(2, 3, 7));
  add_face(body, face(2, 7, 6));
}

static rigid_body_t *make_cube(double x, double y, double z) {
  //   6-------7
  //  /       /|        ^  ^
  // 2-------3 |       z| /y
  // |       | |        |/
  // |       | 5    ----+----->
  // |       |/        /|   x
  // 0-------1        / |
  rigid_body_t *result = make_rigid_body();
  add_point(result, vector(0 + x, 0 + y, 0 + z));
  add_point(result, vector(2 + x, 0 + y, 0 + z));
  add_point(result, vector(0 + x, 0 + y, 2 + z));
  add_point(result, vector(2 + x, 0 + y, 2 + z));
  add_point(result, vector(0 + x, 2 + y, 0 + z));
  add_point(result, vector(2 + x, 2 + y, 0 + z));
  add_point(result, vector(0 + x, 2 + y, 2 + z));
  add_point(result, vector(2 + x, 2 + y, 2 + z));
  add_cube_faces(result);
  return result;
}

static rigid_body_t *make_rotated_cube1(double z) {
  //        7
  //       / \         ^  ^
  //      3   \       z| /y
  //     / \   5       |/
  //    /   \ /    ----+----->
  //   2     1        /|   x
  //    \   /
  //     \ /
  //      0
  rigid_body_t *result = make_rigid_body();
  add_point(result, vector(1, 0, 0 + z));
  add_point(result, vector(2, 0, 1 + z));
  add_point(result, vector(0, 0, 1 + z));
  add_point(result, vector(1, 0, 2 + z));
  add_point(result, vector(1, 2, 0 + z));
  add_point(result, vector(2, 2, 1 + z));
  add_point(result, vector(0, 2, 1 + z));
  add_point(result, vector(1, 2, 2 + z));
  add_cube_faces(result);
  return result;
}

static rigid_body_t *make_rotated_cube2(double z) {
  //    7 ------3        ^  ^
  //   /       / \      z| /y
  //  /       /   1      |/
  // 6-------2   /   ----+----->
  //  \       \ /       /|   x
  //   4-------0
  rigid_body_t *result = make_rigid_body();
  add_point(result, vector(2, 1, 0 + z));
  add_point(result, vector(2, 2, 1 + z));
  add_point(result, vector(2, 0, 1 + z));
  add_point(result, vector(2, 1, 2 + z));
  add_point(result, vector(0, 1, 0 + z));
  add_point(result, vector(0, 2, 1 + z));
  add_point(result, vector(0, 0, 1 + z));
  add_point(result, vector(0, 1, 2 + z));
  add_cube_faces(result);
  return result;
}

static MunitResult test_create(const MunitParameter params[], void *data) {
  munit_assert_int(make_rigid_body()->points.size, ==, 0);
  munit_assert_int(make_rigid_body()->edges.size, ==, 0);
  munit_assert_int(make_rigid_body()->faces.size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_add_point(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_rigid_body();
  add_point(body, vector(2, 3, 5));
  munit_assert_int(body->points.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_add_face(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_rigid_body();
  add_face(body, face(0, 1, 2));
  munit_assert_int(body->faces.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_face_edges(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_rigid_body();
  add_face(body, face(0, 1, 2));
  munit_assert_int(body->edges.size, ==, 3);
  munit_assert_int(get_edge(body->edges)[0].a, ==, 0);
  munit_assert_int(get_edge(body->edges)[0].b, ==, 1);
  munit_assert_int(get_edge(body->edges)[1].a, ==, 1);
  munit_assert_int(get_edge(body->edges)[1].b, ==, 2);
  munit_assert_int(get_edge(body->edges)[2].a, ==, 2);
  munit_assert_int(get_edge(body->edges)[2].b, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_existing_edge(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_rigid_body();
  add_face(body, face(0, 1, 2));
  add_face(body, face(2, 1, 4));
  munit_assert_int(body->edges.size, ==, 5);
  return MUNIT_OK;
}

static MunitResult test_edge_tail(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1.0, 0.0);
  vector_t result = edge_tail(body, edge(1, 2));
  munit_assert_double(result.x, ==, 1.0);
  munit_assert_double(result.y, ==, 0.0);
  munit_assert_double(result.z, ==, 0.0);
  return MUNIT_OK;
}

static MunitResult test_edge_head(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1.0, 0.0);
  vector_t result = edge_head(body, edge(1, 2));
  munit_assert_double(result.x, ==, 0.0);
  munit_assert_double(result.y, ==, 1.0);
  munit_assert_double(result.z, ==, 0.0);
  return MUNIT_OK;
}

static MunitResult test_edge_vector(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1.0, 0.0);
  vector_t result = edge_vector(body, edge(1, 2));
  munit_assert_double(result.x, ==, -1.0);
  munit_assert_double(result.y, ==,  1.0);
  munit_assert_double(result.z, ==,  0.0);
  return MUNIT_OK;
}

static MunitResult test_face_normal(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(2.0, 0.0);
  vector_t result = face_normal(body, face(0, 2, 1));
  munit_assert_double(result.x, ==,  0.0);
  munit_assert_double(result.y, ==,  0.0);
  munit_assert_double(result.z, ==, -1.0);
  return MUNIT_OK;
}

static MunitResult test_face_plane(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1.0, 0.0);
  plane_t plane = face_plane(body, face(2, 1, 0));
  munit_assert_double(plane.point.x, ==, 0.0);
  munit_assert_double(plane.point.y, ==, 1.0);
  munit_assert_double(plane.point.z, ==, 0.0);
  munit_assert_double(plane.normal.x, ==,  0.0);
  munit_assert_double(plane.normal.y, ==,  0.0);
  munit_assert_double(plane.normal.z, ==, -1.0);
  return MUNIT_OK;
}

static MunitResult test_smallest_distance(const MunitParameter params[], void *data) {
  plane_t p = plane(vector(0, 0, 3), vector(0, 0, -1));
  rigid_body_t *body = make_tetrahedron(1.0, 0.0);
  int index = -1;
  munit_assert_double(smallest_distance(p, body, &index), ==, 2);
  munit_assert_int(index, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_largest_distance(const MunitParameter params[], void *data) {
  plane_t p = plane(vector(0, 0, 3), vector(0, 0, -1));
  rigid_body_t *body = make_tetrahedron(1.0, 0.0);
  int index = -1;
  munit_assert_double(largest_distance(p, body, &index), ==, 3);
  munit_assert_int(index, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_best_face(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1.0, 3.0);
  rigid_body_t *other = make_tetrahedron(1.0, 0.0);
  int face_index = -1;
  int point_index = -1;
  munit_assert_double_equal(best_face(body, other, &face_index, &point_index), 2.0, 6);
  munit_assert_int(face_index, ==, 0);
  munit_assert_int(point_index, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_edge_planes(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1, 2);
  rigid_body_t *other = make_tetrahedron(1, 5);
  plane_t p1;
  plane_t p2;
  munit_assert_true(edge_planes(body, edge(1, 3), other, edge(2, 0), &p1, &p2));
  munit_assert_double(p1.point.x, ==, 1);
  munit_assert_double(p1.point.y, ==, 0);
  munit_assert_double(p1.point.z, ==, 2);
  munit_assert_double_equal(p1.normal.x, sqrt(0.5), 6);
  munit_assert_double_equal(p1.normal.y,         0, 6);
  munit_assert_double_equal(p1.normal.z, sqrt(0.5), 6);
  munit_assert_double(p2.point.x, ==, 0);
  munit_assert_double(p2.point.y, ==, 1);
  munit_assert_double(p2.point.z, ==, 5);
  munit_assert_double_equal(p2.normal.x, -sqrt(0.5), 6);
  munit_assert_double_equal(p2.normal.y,          0, 6);
  munit_assert_double_equal(p2.normal.z, -sqrt(0.5), 6);
  return MUNIT_OK;
}

static MunitResult test_check_planes(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1, 2);
  rigid_body_t *other = make_tetrahedron(1, 5);
  plane_t p1;
  plane_t p2;
  munit_assert_true(edge_planes(body, edge(1, 3), other, edge(0, 2), &p1, &p2));
  munit_assert_double_equal(p1.normal.x, sqrt(0.5), 6);
  munit_assert_double_equal(p1.normal.y,         0, 6);
  munit_assert_double_equal(p1.normal.z, sqrt(0.5), 6);
  munit_assert_double(p2.point.x, ==, 0);
  munit_assert_double(p2.point.y, ==, 0);
  munit_assert_double(p2.point.z, ==, 5);
  munit_assert_double_equal(p2.normal.x, -sqrt(0.5), 6);
  munit_assert_double_equal(p2.normal.y,          0, 6);
  munit_assert_double_equal(p2.normal.z, -sqrt(0.5), 6);
  return MUNIT_OK;
}

static MunitResult test_reject_planes(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1, 2);
  rigid_body_t *other = make_tetrahedron(1, 5);
  plane_t p1;
  plane_t p2;
  munit_assert_false(edge_planes(body, edge(0, 1), other, edge(0, 2), &p1, &p2));
  return MUNIT_OK;
}

static MunitResult test_parallel_edges(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1, 2);
  rigid_body_t *other = make_tetrahedron(1, 5);
  plane_t p1;
  plane_t p2;
  munit_assert_false(edge_planes(body, edge(0, 1), other, edge(0, 1), &p1, &p2));
  return MUNIT_OK;
}

static MunitResult test_best_edge_pair(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1, 2);
  rigid_body_t *other = make_tetrahedron(1, 5);
  int edge1_index = -1;
  int edge2_index = -1;
  munit_assert_double_equal(best_edge_pair(body, other, &edge1_index, &edge2_index), sqrt(2), 6);
  edge_t edge1 = get_edge(body->edges)[edge1_index];
  edge_t edge2 = get_edge(other->edges)[edge2_index];
  munit_assert_int(edge1.a, ==, 3);
  munit_assert_int(edge1.b, ==, 2);
  munit_assert_int(edge2.a, ==, 1);
  munit_assert_int(edge2.b, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_no_penetration(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1, 2);
  plane_t p = plane(vector(0, 0, 0), vector(0, 0, 1));
  munit_assert_int(penetration_candidates(p, body).size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_penetration(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_tetrahedron(1, 2);
  plane_t p = plane(vector(0, 0, 2), vector(0, 0, 1));
  munit_assert_int(penetration_candidates(p, body).size, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_separating_plane(const MunitParameter params[], void *data) {
  rigid_body_t *cube = make_cube(0, 0, 0);
  rigid_body_t *tetrahedron = make_tetrahedron(2, 4);
  double distance;
  plane_t result = separating_plane(cube, tetrahedron, &distance);
  munit_assert_double(distance, ==, 2.0);
  munit_assert_double(result.point.z, ==, 3.0);
  munit_assert_double(result.normal.x, ==, 0.0);
  munit_assert_double(result.normal.y, ==, 0.0);
  munit_assert_double(result.normal.z, ==, 1.0);
  return MUNIT_OK;
}

static MunitResult test_separating_plane2(const MunitParameter params[], void *data) {
  rigid_body_t *tetrahedron = make_tetrahedron(2, 0);
  rigid_body_t *cube = make_cube(0, 0, 4);
  double distance;
  plane_t result = separating_plane(tetrahedron, cube, &distance);
  munit_assert_double(distance, ==, 2.0);
  munit_assert_double(result.point.z, ==, 3.0);
  munit_assert_double(result.normal.x, ==, 0.0);
  munit_assert_double(result.normal.y, ==, 0.0);
  munit_assert_double(result.normal.z, ==, 1.0);
  return MUNIT_OK;
}

static MunitResult test_separating_plane3(const MunitParameter params[], void *data) {
  rigid_body_t *cube1 = make_rotated_cube1(0);
  rigid_body_t *cube2 = make_rotated_cube2(4);
  double distance;
  plane_t result = separating_plane(cube1, cube2, &distance);
  munit_assert_double(distance, ==, 2.0);
  munit_assert_double(result.point.z, ==, 3.0);
  munit_assert_double(result.normal.x, ==, 0.0);
  munit_assert_double(result.normal.y, ==, 0.0);
  munit_assert_double(result.normal.z, ==, 1.0);
  return MUNIT_OK;
}

static MunitResult test_contact_points(const MunitParameter params[], void *data) {
  rigid_body_t *cube1 = make_cube(0, 0, 0);
  rigid_body_t *cube2 = make_cube(1, 1, 1.9);
  double distance;
  vector_t normal;
  list_t points = contact_points(cube1, cube2, &distance, &normal);
  munit_assert_double_equal(distance, -0.1, 6);
  munit_assert_int(points.size, ==, 4);
  munit_assert_double(get_vector(points)[0].x, ==, 2);
  munit_assert_double(get_vector(points)[0].y, ==, 2);
  munit_assert_double(get_vector(points)[0].z, ==, 1.95);
  munit_assert_double(get_vector(points)[1].x, ==, 1);
  munit_assert_double(get_vector(points)[1].y, ==, 2);
  munit_assert_double(get_vector(points)[1].z, ==, 1.95);
  munit_assert_double(get_vector(points)[2].x, ==, 1);
  munit_assert_double(get_vector(points)[2].y, ==, 1);
  munit_assert_double(get_vector(points)[2].z, ==, 1.95);
  munit_assert_double(get_vector(points)[3].x, ==, 2);
  munit_assert_double(get_vector(points)[3].y, ==, 1);
  munit_assert_double(get_vector(points)[3].z, ==, 1.95);
  munit_assert_double(normal.x, ==, 0);
  munit_assert_double(normal.y, ==, 0);
  munit_assert_double(normal.z, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_contact_points2(const MunitParameter params[], void *data) {
  rigid_body_t *cube1 = make_rotated_cube1(0);
  rigid_body_t *cube2 = make_rotated_cube2(2);
  double distance;
  vector_t normal;
  list_t points = contact_points(cube1, cube2, &distance, &normal);
  munit_assert_double(distance, ==, 0);
  munit_assert_int(points.size, ==, 1);
  munit_assert_double(get_vector(points)[0].x, ==, 1);
  munit_assert_double(get_vector(points)[0].y, ==, 1);
  munit_assert_double(get_vector(points)[0].z, ==, 2);
  munit_assert_double(normal.x, ==, 0);
  munit_assert_double(normal.y, ==, 0);
  munit_assert_double(normal.z, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_empty_objects(const MunitParameter params[], void *data) {
  rigid_body_t *body1 = make_rigid_body();
  rigid_body_t *body2 = make_rigid_body();
  list_t points = contact_points(body1, body2, NULL, NULL);
  munit_assert_int(points.size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_no_contact(const MunitParameter params[], void *data) {
  rigid_body_t *cube1 = make_cube(0, 0, 0);
  rigid_body_t *cube2 = make_cube(0, 0, 3);
  double distance;
  vector_t normal;
  list_t points = contact_points(cube1, cube2, &distance, &normal);
  munit_assert_double(distance, ==, 1);
  munit_assert_int(points.size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_transform(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_cube(0, 0, 0);
  rigid_body_t *result = transform_body(body, quaternion_rotation(M_PI / 2, vector(1, 0, 0)), vector(1, 2, 3));
  list_t points = result->points;
  munit_assert_memory_equal(sizeof(list_t), &result->edges, &body->edges);
  munit_assert_memory_equal(sizeof(list_t), &result->faces, &body->faces);
  munit_assert_double_equal(get_vector(points)[0].x, 1, 6);
  munit_assert_double_equal(get_vector(points)[0].y, 2, 6);
  munit_assert_double_equal(get_vector(points)[0].z, 3, 6);
  munit_assert_double_equal(get_vector(points)[7].x, 3, 6);
  munit_assert_double_equal(get_vector(points)[7].y, 0, 6);
  munit_assert_double_equal(get_vector(points)[7].z, 5, 6);
  return MUNIT_OK;
}

static MunitResult test_contacts(const MunitParameter params[], void *data) {
  rigid_body_t *cube1 = make_cube(0, 0, 0);
  rigid_body_t *cube2 = make_cube(1, 1, 1.9);
  state_t *s1 = state(vector(0, 0, 0), vector(0, 0, 0.1), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  state_t *s2 = state(vector(1, 1, 1.9), vector(0, 0, -0.1), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  list_t contacts_ = contacts(2, 3, cube1, cube2, 0, 1, s1, s2);
  munit_assert_int(contacts_.size, ==, 4);
  contact_t c0 = get_contact(contacts_)[0];
  munit_assert_int(c0.i, ==, 2);
  munit_assert_int(c0.j, ==, 3);
  munit_assert_double(c0.normal.x, ==, 0);
  munit_assert_double(c0.normal.y, ==, 0);
  munit_assert_double(c0.normal.z, ==, 1);
  munit_assert_double(c0.point.x, ==, 2);
  munit_assert_double(c0.point.y, ==, 2);
  munit_assert_double(c0.point.z, ==, 1.95);
  munit_assert_double_equal(c0.distance, -0.1, 6);
  munit_assert_double_equal(c0.normal_speed, -0.2, 6);
  return MUNIT_OK;
}

MunitTest test_rigid_body[] = {
  {"/create"           , test_create           , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_point"        , test_add_point        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_face"         , test_add_face         , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/face_edges"       , test_face_edges       , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/existing_edge"    , test_existing_edge    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/edge_tail"        , test_edge_tail        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/edge_head"        , test_edge_head        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/edge_vector"      , test_edge_vector      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/face_normal"      , test_face_normal      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/face_plane"       , test_face_plane       , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/smallest_distance", test_smallest_distance, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/largest_distance" , test_largest_distance , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/best_face"        , test_best_face        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/edge_planes"      , test_edge_planes      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/check_planes"     , test_check_planes     , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/reject_planes"    , test_reject_planes    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/parallel_edges"   , test_parallel_edges   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/best_edge_pair"   , test_best_edge_pair   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_penetration"   , test_no_penetration   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/penetration"      , test_penetration      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/separating_plane" , test_separating_plane , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/separating_plane2", test_separating_plane2, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/separating_plane3", test_separating_plane3, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/contact_points"   , test_contact_points   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/contact_points2"  , test_contact_points2  , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/empty_objects"    , test_empty_objects    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/no_contact"       , test_no_contact       , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/transform"        , test_transform        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/contacts"         , test_contacts         , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                , NULL                  , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
