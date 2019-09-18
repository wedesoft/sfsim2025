#include "sfsim2025/rigid_body.h"
#include "test_rigid_body.h"
#include "test_helper.h"


static MunitResult test_create(const MunitParameter params[], void *data)
{
  munit_assert_int(make_rigid_body()->points->size, ==, 0);
  munit_assert_int(make_rigid_body()->edges->size, ==, 0);
  munit_assert_int(make_rigid_body()->faces->size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_add_point(const MunitParameter params[], void *data)
{
  rigid_body_t *body = make_rigid_body();
  add_point(body, vector(2, 3, 5));
  munit_assert_int(body->points->size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_add_face(const MunitParameter params[], void *data)
{
  rigid_body_t *body = make_rigid_body();
  add_face(body, face(0, 1, 2));
  munit_assert_int(body->faces->size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_face_edges(const MunitParameter params[], void *data)
{
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

static MunitResult test_existing_edge(const MunitParameter params[], void *data)
{
  rigid_body_t *body = make_rigid_body();
  add_face(body, face(0, 1, 2));
  add_face(body, face(2, 1, 4));
  munit_assert_int(body->edges->size, ==, 5);
  return MUNIT_OK;
}

static rigid_body_t *make_object(double s, double z)
{
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

static MunitResult test_face_normal(const MunitParameter params[], void *data)
{
  rigid_body_t *body = make_object(2.0, 0.0);
  vector_t result = face_normal(body, face(0, 2, 1));
  munit_assert_double(result.x, ==,  0.0);
  munit_assert_double(result.y, ==,  0.0);
  munit_assert_double(result.z, ==, -1.0);
  return MUNIT_OK;
}

static MunitResult test_face_plane(const MunitParameter params[], void *data)
{
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

static MunitResult test_smallest_distance(const MunitParameter params[], void *data)
{
  plane_t p = plane(vector(0, 0, 3), vector(0, 0, -1));
  rigid_body_t *body = make_object(1.0, 0.0);
  int index = -1;
  munit_assert_double(smallest_distance(p, body, &index), ==, 2);
  munit_assert_int(index, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_best_face(const MunitParameter params[], void *data)
{
  rigid_body_t *body = make_object(1.0, 3.0);
  rigid_body_t *other = make_object(1.0, 0.0);
  int face_index = -1;
  int point_index = -1;
  munit_assert_double(best_face(body, other, &face_index, &point_index), ==, 2.0);
  munit_assert_int(face_index, ==, 0);
  munit_assert_int(point_index, ==, 3);
  return MUNIT_OK;
}

MunitTest test_rigid_body[] = {
  {"/create"           , test_create           , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_point"        , test_add_point        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/add_face"         , test_add_face         , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/face_edges"       , test_face_edges       , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/existing_edge"    , test_existing_edge    , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/face_normal"      , test_face_normal      , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/face_plane"       , test_face_plane       , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/smallest_distance", test_smallest_distance, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/best_face"        , test_best_face        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                , NULL                  , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
