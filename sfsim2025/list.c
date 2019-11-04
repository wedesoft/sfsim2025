#include <stdbool.h>
#include <string.h>
#include <gc.h>
#include "list.h"

static void grow_list(list_t *list, size_t element_size, bool atomic) {
  if (list->buffer_size < (list->size + 1) * element_size) {
    list->buffer_size = list->buffer_size ? 2 * list->buffer_size : element_size;
    GLuint *space = atomic ? GC_MALLOC_ATOMIC(list->buffer_size) : GC_MALLOC(list->buffer_size);
    memcpy(space, list->element, list->size * element_size);
    list->element = space;
  };
}

void append_gluint(list_t *list, GLuint value) {
  grow_list(list, sizeof(GLuint), true);
  ((GLuint *)list->element)[list->size++] = value;
}

void append_glfloat(list_t *list, GLfloat value) {
  grow_list(list, sizeof(GLfloat), true);
  ((GLfloat *)list->element)[list->size++] = value;
}

void append_pointer(list_t *list, void *value) {
  grow_list(list, sizeof(void *), false);
  ((void **)list->element)[list->size++] = value;
}

void append_vector(list_t *list, vector_t value) {
  grow_list(list, sizeof(vector_t), true);
  ((vector_t *)list->element)[list->size++] = value;
}

void append_face(list_t *list, face_t value) {
  grow_list(list, sizeof(face_t), true);
  ((face_t *)list->element)[list->size++] = value;
}

void append_edge(list_t *list, edge_t value) {
  grow_list(list, sizeof(edge_t), true);
  ((edge_t *)list->element)[list->size++] = value;
}

void append_coordinate(list_t *list, coordinate_t value) {
  grow_list(list, sizeof(coordinate_t), true);
  ((coordinate_t *)list->element)[list->size++] = value;
}

void append_contact(list_t *list, contact_t value) {
  grow_list(list, sizeof(contact_t), true);
  ((contact_t *)list->element)[list->size++] = value;
}

void append_body(list_t *list, body_t value) {
  grow_list(list, sizeof(body_t), true);
  ((body_t *)list->element)[list->size++] = value;
}

void append_forces(list_t *list, forces_t value) {
  grow_list(list, sizeof(forces_t), true);
  ((forces_t *)list->element)[list->size++] = value;
}
