#pragma once
#include <gc.h>
#include <stdio.h>


typedef struct {
  int height;
  int width;
  unsigned char *data;
} image_t;

static inline image_t allocate_image(int height, int width) {
  return (image_t){.height = height, .width = width, .data = GC_MALLOC_ATOMIC(height * width * 3)};
}

image_t read_image(const char *file_name);

void write_image(image_t image, const char *file_name);

image_t crop_image(image_t image, int y, int x, int height, int width);

void mkdir_p(const char *path);

static inline char *tilepath(const char *prefix, int level, int y, int x, const char *suffix) {
  char *result = GC_MALLOC_ATOMIC(256);
  snprintf(result, 256, "%s/%d/%d/%d%s", prefix, level, x, y, suffix);
  return result;
}

static inline char *cubepath(const char *prefix, int face, int level, int y, int x, const char *suffix) {
  char *result = GC_MALLOC_ATOMIC(256);
  snprintf(result, 256, "%s/%d/%d/%d/%d%s", prefix, face, level, x, y, suffix);
  return result;
}

image_t scale_image(image_t image);
