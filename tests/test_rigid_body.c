#include "sfsim2025/rigid_body.h"
#include "test_rigid_body.h"
#include "test_helper.h"


static MunitResult test_create(const MunitParameter params[], void *data) {
  munit_assert_int(make_rigid_body()->points->size, ==, 0);
  munit_assert_int(make_rigid_body()->edges->size, ==, 0);
  munit_assert_int(make_rigid_body()->faces->size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_add_point(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_rigid_body();
  add_point(body, vector(2, 3, 5));
  munit_assert_int(body->points->size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_add_face(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_rigid_body();
  add_face(body, face(0, 1, 2));
  munit_assert_int(body->faces->size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_face_edges(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_rigid_body();
  add_face(body, face(0, 1, 2));
  munit_assert_int(body->edges->size, ==, 3);
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
  munit_assert_int(body->edges->size, ==, 5);
  return MUNIT_OK;
}

static rigid_body_t *make_object(double s, double z) {
  rigid_body_t *result = make_rigid_body();
  add_point(result, vector(0, 0, z    ));
  add_point(result, vector(s, 0, z    ));
  add_point(result, vector(0, s, z    ));
  add_point(result, vector(0, 0, z + s));
  add_face(result, face(0, 2, 1));
  add_face(result, face(0, 3, 2));
  add_face(result, face(0, 1, 3));
  add_face(result, face(1, 2, 3));
  return result;
}

static MunitResult test_edge_tail(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_object(1.0, 0.0);
  vector_t result = edge_tail(body, edge(1, 2));
  munit_assert_double(result.x, ==, 1.0);
  munit_assert_double(result.y, ==, 0.0);
  munit_assert_double(result.z, ==, 0.0);
  return MUNIT_OK;
}

static MunitResult test_edge_head(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_object(1.0, 0.0);
  vector_t result = edge_head(body, edge(1, 2));
  munit_assert_double(result.x, ==, 0.0);
  munit_assert_double(result.y, ==, 1.0);
  munit_assert_double(result.z, ==, 0.0);
  return MUNIT_OK;
}

static MunitResult test_edge_vector(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_object(1.0, 0.0);
  vector_t result = edge_vector(body, edge(1, 2));
  munit_assert_double(result.x, ==, -1.0);
  munit_assert_double(result.y, ==,  1.0);
  munit_assert_double(result.z, ==,  0.0);
  return MUNIT_OK;
}

static MunitResult test_face_normal(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_object(2.0, 0.0);
  vector_t result = face_normal(body, face(0, 2, 1));
  munit_assert_double(result.x, ==,  0.0);
  munit_assert_double(result.y, ==,  0.0);
  munit_assert_double(result.z, ==, -1.0);
  return MUNIT_OK;
}

static MunitResult test_face_plane(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_object(1.0, 0.0);
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
  rigid_body_t *body = make_object(1.0, 0.0);
  int index = -1;
  munit_assert_double(smallest_distance(p, body, &index), ==, 2);
  munit_assert_int(index, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_largest_distance(const MunitParameter params[], void *data) {
  plane_t p = plane(vector(0, 0, 3), vector(0, 0, -1));
  rigid_body_t *body = make_object(1.0, 0.0);
  int index = -1;
  munit_assert_double(largest_distance(p, body, &index), ==, 3);
  munit_assert_int(index, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_best_face(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_object(1.0, 3.0);
  rigid_body_t *other = make_object(1.0, 0.0);
  int face_index = -1;
  int point_index = -1;
  munit_assert_double(best_face(body, other, &face_index, &point_index), ==, 2.0);
  munit_assert_int(face_index, ==, 0);
  munit_assert_int(point_index, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_edge_planes(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_object(1, 2);
  rigid_body_t *other = make_object(1, 5);
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
  rigid_body_t *body = make_object(1, 2);
  rigid_body_t *other = make_object(1, 5);
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
  rigid_body_t *body = make_object(1, 2);
  rigid_body_t *other = make_object(1, 5);
  plane_t p1;
  plane_t p2;
  munit_assert_false(edge_planes(body, edge(0, 1), other, edge(0, 2), &p1, &p2));
  return MUNIT_OK;
}

static MunitResult test_parallel_edges(const MunitParameter params[], void *data) {
  rigid_body_t *body = make_object(1, 2);
  rigid_body_t *other = make_object(1, 5);
  plane_t p1;
  plane_t p2;
  munit_assert_false(edge_planes(body, edge(0, 1), other, edge(0, 1), &p1, &p2));
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
  {NULL                , NULL                  , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
