#include <stdbool.h>
#include <string.h>
#include <gc.h>
#include "list.h"

void grow_list(list_t *list, size_t element_size, bool atomic) {
  if (list->buffer_size < (list->size + 1) * element_size) {
    list->buffer_size = list->buffer_size ? 2 * list->buffer_size : element_size;
    GLuint *space = atomic ? GC_MALLOC_ATOMIC(list->buffer_size) : GC_MALLOC(list->buffer_size);
    memcpy(space, list->element, list->size * element_size);
    list->element = space;
  };
}

void remove_from_list(list_t *list, size_t element_size, int index) {
  int n = list->size - index - 1;
  memmove(list->element + index * element_size, list->element + (index + 1) * element_size, n * element_size);
  list->size -= 1;
}
