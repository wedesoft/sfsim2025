#pragma once
#include <GL/gl.h>
#include "coordinate.h"
#include "edge.h"
#include "face.h"
#include "vector.h"


typedef struct {
  size_t size;
  size_t buffer_size;
  void *element;
} list_t;

list_t *make_list(void);

void append_gluint(list_t *list, GLuint value);

static inline GLuint *get_gluint(list_t *list) { return (GLuint *)list->element; }

void append_glfloat(list_t *list, GLfloat value);

static inline GLfloat *get_glfloat(list_t *list) { return (GLfloat *)list->element; }

void append_pointer(list_t *list, void *value);

static inline void **get_pointer(list_t *list) { return (void **)list->element; }

void append_vector(list_t *list, vector_t value);

static inline vector_t *get_vector(list_t *list) { return (vector_t *)list->element; }

void append_face(list_t *list, face_t value);

static inline face_t *get_face(list_t *list) { return (face_t *)list->element; }

void append_edge(list_t *list, edge_t value);

static inline edge_t *get_edge(list_t *list) { return (edge_t *)list->element; }

void append_coordinate(list_t *list, coordinate_t value);

static inline coordinate_t *get_coordinate(list_t *list) { return (coordinate_t *)list->element; }
