#pragma once
#include <GL/gl.h>
#include "point.h"
#include "face.h"
#include "edge.h"


typedef struct {
  int size;
  int buffer_size;
  void *element;
} list_t;

list_t *make_list(void);

void append_gluint(list_t *list, GLuint value);

inline GLuint *get_gluint(list_t *list) { return (GLuint *)list->element; }

void append_glfloat(list_t *list, GLfloat value);

inline GLfloat *get_glfloat(list_t *list) { return (GLfloat *)list->element; }

void append_pointer(list_t *list, void *value);

inline void **get_pointer(list_t *list) { return (void **)list->element; }

void append_point(list_t *list, point_t value);

inline point_t *get_point(list_t *list) { return (point_t *)list->element; }

void append_face(list_t *list, face_t value);

inline face_t *get_face(list_t *list) { return (face_t *)list->element; }

void append_edge(list_t *list, edge_t value);

inline edge_t *get_edge(list_t *list) { return (edge_t *)list->element; }
