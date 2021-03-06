#include <GL/gl.h>
#include "sfsim2025/list.h"
#include "test_list.h"
#include "test_helper.h"


static MunitResult test_zero_size(const MunitParameter params[], void *data) {
  list_t list = make_list();
  munit_assert_int(list.size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_append_gluint(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_gluint(&list, 123);
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_gluint(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_gluint(&list, 123);
  munit_assert_int(get_gluint(list)[0], ==, 123);
  return MUNIT_OK;
}

static MunitResult test_append_two(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_gluint(&list, 235);
  append_gluint(&list, 357);
  munit_assert_int(list.size, ==, 2);
  return MUNIT_OK;
}

static MunitResult test_get_two(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_gluint(&list, 235);
  append_gluint(&list, 357);
  munit_assert_int(get_gluint(list)[0], ==, 235);
  munit_assert_int(get_gluint(list)[1], ==, 357);
  return MUNIT_OK;
}

static MunitResult test_initial_storage(const MunitParameter params[], void *data) {
  munit_assert_int(make_list().buffer_size, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_allocation(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_gluint(&list, 235);
  munit_assert_int(list.buffer_size, ==, sizeof(GLuint));
  return MUNIT_OK;
}

static MunitResult test_reallocation(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_gluint(&list, 235);
  append_gluint(&list, 357);
  munit_assert_int(list.buffer_size, ==, 2 * sizeof(GLuint));
  return MUNIT_OK;
}

static MunitResult test_double_size(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_gluint(&list, 235);
  append_gluint(&list, 357);
  append_gluint(&list, 5711);
  munit_assert_int(list.buffer_size, ==, 4 * sizeof(GLuint));
  return MUNIT_OK;
}

static MunitResult test_append_glfloat(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_glfloat(&list, 2.5f);
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_glfloat(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_glfloat(&list, 7.5f);
  munit_assert_float(get_glfloat(list)[0], ==, 7.5f);
  return MUNIT_OK;
}

static MunitResult test_append_pointer(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_pointer(&list, NULL);
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_remove_pointer(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_pointer(&list, "a");
  append_pointer(&list, "b");
  append_pointer(&list, "c");
  remove_pointer(&list, 1);
  munit_assert_string_equal(get_pointer(list)[0], "a");
  munit_assert_string_equal(get_pointer(list)[1], "c");
  return MUNIT_OK;
}

static MunitResult test_get_pointer(const MunitParameter params[], void *data) {
  char test = 42;
  list_t list = make_list();
  append_pointer(&list, &test);
  munit_assert_ptr(get_pointer(list)[0], ==, &test);
  return MUNIT_OK;
}

static MunitResult test_append_vector(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_vector(&list, vector(2, 3, 5));
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_vector(const MunitParameter params[], void *data) {
  vector_t p = vector(2, 3, 5);
  list_t list = make_list();
  append_vector(&list, p);
  munit_assert_double(get_vector(list)[0].x, ==, 2);
  munit_assert_double(get_vector(list)[0].y, ==, 3);
  munit_assert_double(get_vector(list)[0].z, ==, 5);
  return MUNIT_OK;
}

static MunitResult test_append_face(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_face(&list, face(0, 1, 2));
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_face(const MunitParameter params[], void *data) {
  face_t f = face(0, 1, 2);
  list_t list = make_list();
  append_face(&list, f);
  munit_assert_int(get_face(list)[0].a, ==, 0);
  munit_assert_int(get_face(list)[0].b, ==, 1);
  munit_assert_int(get_face(list)[0].c, ==, 2);
  return MUNIT_OK;
}

static MunitResult test_append_edge(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_edge(&list, edge(2, 3));
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_edge(const MunitParameter params[], void *data) {
  edge_t e = edge(2, 3);
  list_t list = make_list();
  append_edge(&list, e);
  munit_assert_int(get_edge(list)[0].a, ==, 2);
  munit_assert_int(get_edge(list)[0].b, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_append_coordinate(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_coordinate(&list, coordinate(2, 3));
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_coordinate(const MunitParameter params[], void *data) {
  coordinate_t c = coordinate(2, 3);
  list_t list = make_list();
  append_coordinate(&list, c);
  munit_assert_int(get_coordinate(list)[0].u, ==, 2);
  munit_assert_int(get_coordinate(list)[0].v, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_append_contact_candidate(const MunitParameter params[], void *data) {
  contact_candidate_t c = contact_candidate(2, 3, uniform_friction(0.5));
  list_t list = make_list();
  append_contact_candidate(&list, c);
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_contact_candidate(const MunitParameter params[], void *data) {
  contact_candidate_t c = contact_candidate(2, 3, uniform_friction(0.5));
  list_t list = make_list();
  append_contact_candidate(&list, c);
  munit_assert_int(get_contact_candidate(list)[0].i, ==, 2);
  munit_assert_int(get_contact_candidate(list)[0].j, ==, 3);
  munit_assert_double(get_contact_candidate(list)[0].friction.uniform, ==, 0.5);
  return MUNIT_OK;
}

static MunitResult test_append_contact(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(1, 2, 3), vector(3, 4, 5), 0.1, 0, -2.0, uniform_friction(1.0));
  list_t list = make_list();
  append_contact(&list, c);
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_contact(const MunitParameter params[], void *data) {
  contact_t c = contact(2, 3, vector(1, 2, 3), vector(3, 4, 5), 0.1, 0, -2.0, uniform_friction(1.0));
  list_t list = make_list();
  append_contact(&list, c);
  munit_assert_int(get_contact(list)[0].i, ==, 2);
  munit_assert_int(get_contact(list)[0].j, ==, 3);
  return MUNIT_OK;
}

static MunitResult test_append_body(const MunitParameter params[], void *data) {
  body_t b = body(1.0, diagonal(2, 3, 5));
  list_t list = make_list();
  append_body(&list, b);
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_body(const MunitParameter params[], void *data) {
  body_t b = body(1.0, diagonal(2, 3, 5));
  list_t list = make_list();
  append_body(&list, b);
  munit_assert_double(get_body(list)[0].mass, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_append_force(const MunitParameter params[], void *data) {
  force_t f = gravitation(0, 1);
  list_t list = make_list();
  append_force(&list, f);
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_force(const MunitParameter params[], void *data) {
  force_t f = gravitation(0, 1);
  list_t list = make_list();
  append_force(&list, f);
  munit_assert_int(get_force(list)[0].j, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_append_joint(const MunitParameter params[], void *data) {
  joint_t f = ball_in_socket(1, 2, vector(2, 3, 5), vector(3, 5, 7));
  list_t list = make_list();
  append_joint(&list, f);
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_joint(const MunitParameter params[], void *data) {
  joint_t f = ball_in_socket(1, 2, vector(2, 3, 5), vector(3, 5, 7));
  list_t list = make_list();
  append_joint(&list, f);
  munit_assert_int(get_joint(list)[0].i, ==, 1);
  munit_assert_int(get_joint(list)[0].j, ==, 2);
  return MUNIT_OK;
}

static MunitResult test_append_image(const MunitParameter params[], void *data) {
  image_t image = allocate_image(32, 32);
  list_t list = make_list();
  append_image(&list, image);
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_image(const MunitParameter params[], void *data) {
  image_t image = allocate_image(32, 32);
  list_t list = make_list();
  append_image(&list, image);
  munit_assert_int(get_image(list)[0].width, ==, 32);
  return MUNIT_OK;
}

static MunitResult test_remove_image(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_image(&list, allocate_image(2, 2));
  append_image(&list, allocate_image(3, 3));
  append_image(&list, allocate_image(5, 5));
  remove_image(&list, 1);
  munit_assert_int(list.size, ==, 2);
  munit_assert_int(get_image(list)[0].width, ==, 2);
  munit_assert_int(get_image(list)[1].width, ==, 5);
  return MUNIT_OK;
}

static MunitResult test_append_elevation(const MunitParameter params[], void *data) {
  elevation_t elevation = allocate_elevation(32, 32);
  list_t list = make_list();
  append_elevation(&list, elevation);
  munit_assert_int(list.size, ==, 1);
  return MUNIT_OK;
}

static MunitResult test_get_elevation(const MunitParameter params[], void *data) {
  elevation_t elevation = allocate_elevation(32, 32);
  list_t list = make_list();
  append_elevation(&list, elevation);
  munit_assert_int(get_elevation(list)[0].width, ==, 32);
  return MUNIT_OK;
}

static MunitResult test_remove_elevation(const MunitParameter params[], void *data) {
  list_t list = make_list();
  append_elevation(&list, allocate_elevation(2, 2));
  append_elevation(&list, allocate_elevation(3, 3));
  append_elevation(&list, allocate_elevation(5, 5));
  remove_elevation(&list, 1);
  munit_assert_int(list.size, ==, 2);
  munit_assert_int(get_elevation(list)[0].width, ==, 2);
  munit_assert_int(get_elevation(list)[1].width, ==, 5);
  return MUNIT_OK;
}

MunitTest test_list[] = {
  {"/zero_size"               , test_zero_size               , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_gluint"           , test_append_gluint           , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_gluint"              , test_get_gluint              , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_two"              , test_append_two              , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_two"                 , test_get_two                 , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/initial_storage"         , test_initial_storage         , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/allocation"              , test_allocation              , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/reallocation"            , test_reallocation            , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/double_size"             , test_double_size             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_glfloat"          , test_append_glfloat          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_glfloat"             , test_get_glfloat             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_pointer"          , test_append_pointer          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/remove_pointer"          , test_remove_pointer          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_pointer"             , test_get_pointer             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_vector"           , test_append_vector           , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_vector"              , test_get_vector              , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_face"             , test_append_face             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_face"                , test_get_face                , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_edge"             , test_append_edge             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_edge"                , test_get_edge                , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_coordinate"       , test_append_coordinate       , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_coordinate"          , test_get_coordinate          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_contact_candidate", test_append_contact_candidate, test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_contact_candidate"   , test_get_contact_candidate   , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_contact"          , test_append_contact          , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_contact"             , test_get_contact             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_body"             , test_append_body             , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_body"                , test_get_body                , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_force"            , test_append_force            , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_force"               , test_get_force               , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_joint"            , test_append_joint            , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_joint"               , test_get_joint               , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_image"            , test_append_image            , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_image"               , test_get_image               , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/remove_image"            , test_remove_image            , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/append_elevation"        , test_append_elevation        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/get_elevation"           , test_get_elevation           , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {"/remove_elevation"        , test_remove_elevation        , test_setup_gc, test_teardown_gc, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL                       , NULL                         , NULL         , NULL            , MUNIT_TEST_OPTION_NONE, NULL}
};
