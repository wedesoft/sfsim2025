#pragma once
#include <GL/gl.h>
#include "body.h"
#include "contact.h"
#include "coordinate.h"
#include "edge.h"
#include "face.h"
#include "force.h"
#include "image.h"
#include "joint.h"
#include "vector.h"


typedef struct {
  size_t size;
  size_t buffer_size;
  void *element;
} list_t;

static inline list_t make_list(void) {
  return (list_t){.size = 0, .buffer_size = 0, .element = 0};
}

void grow_list(list_t *list, size_t element_size, bool atomic);

void remove_from_list(list_t *list, size_t element_size, int index);

static inline void append_gluint(list_t *list, GLuint value) {
  grow_list(list, sizeof(GLuint), true);
  ((GLuint *)list->element)[list->size++] = value;
}

static inline GLuint *get_gluint(list_t list) {
  return (GLuint *)list.element;
}

static inline void append_glfloat(list_t *list, GLfloat value) {
  grow_list(list, sizeof(GLfloat), true);
  ((GLfloat *)list->element)[list->size++] = value;
}

static inline GLfloat *get_glfloat(list_t list) {
  return (GLfloat *)list.element;
}

static inline void append_pointer(list_t *list, void *value) {
  grow_list(list, sizeof(void *), false);
  ((void **)list->element)[list->size++] = value;
}

static inline void **get_pointer(list_t list) {
  return (void **)list.element;
}

static inline void remove_pointer(list_t *list, int index) {
  remove_from_list(list, sizeof(void *), index);
}

static inline void append_vector(list_t *list, vector_t value) {
  grow_list(list, sizeof(vector_t), true);
  ((vector_t *)list->element)[list->size++] = value;
}

static inline vector_t *get_vector(list_t list) {
  return (vector_t *)list.element;
}

static inline void append_face(list_t *list, face_t value) {
  grow_list(list, sizeof(face_t), true);
  ((face_t *)list->element)[list->size++] = value;
}

static inline face_t *get_face(list_t list) {
  return (face_t *)list.element;
}

static inline void append_edge(list_t *list, edge_t value) {
  grow_list(list, sizeof(edge_t), true);
  ((edge_t *)list->element)[list->size++] = value;
}

static inline edge_t *get_edge(list_t list) {
  return (edge_t *)list.element;
}

static inline void append_coordinate(list_t *list, coordinate_t value) {
  grow_list(list, sizeof(coordinate_t), true);
  ((coordinate_t *)list->element)[list->size++] = value;
}

static inline coordinate_t *get_coordinate(list_t list) {
  return (coordinate_t *)list.element;
}

static inline void append_contact_candidate(list_t *list, contact_candidate_t value) {
  grow_list(list, sizeof(contact_candidate_t), true);
  ((contact_candidate_t *)list->element)[list->size++] = value;
}

static inline contact_candidate_t *get_contact_candidate(list_t list) {
  return (contact_candidate_t *)list.element;
}

static inline void append_contact(list_t *list, contact_t value) {
  grow_list(list, sizeof(contact_t), true);
  ((contact_t *)list->element)[list->size++] = value;
}

static inline contact_t *get_contact(list_t list) {
  return (contact_t *)list.element;
}

static inline void append_body(list_t *list, body_t value) {
  grow_list(list, sizeof(body_t), true);
  ((body_t *)list->element)[list->size++] = value;
}

static inline body_t *get_body(list_t list) {
  return (body_t *)list.element;
}

static inline void append_force(list_t *list, force_t value) {
  grow_list(list, sizeof(force_t), true);
  ((force_t *)list->element)[list->size++] = value;
}

static inline force_t *get_force(list_t list) {
  return (force_t *)list.element;
}

static inline void append_joint(list_t *list, joint_t value) {
  grow_list(list, sizeof(joint_t), true);
  ((joint_t *)list->element)[list->size++] = value;
}

static inline joint_t *get_joint(list_t list) {
  return (joint_t *)list.element;
}

static inline void append_image(list_t *list, image_t value) {
  grow_list(list, sizeof(image_t), false);
  ((image_t *)list->element)[list->size++] = value;
}

static inline image_t *get_image(list_t list) {
  return (image_t *)list.element;
}

static inline void remove_image(list_t *list, int index) {
  remove_from_list(list, sizeof(image_t), index);
}
