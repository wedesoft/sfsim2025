#pragma once


typedef struct {
  int height;
  int width;
  char *data;
} image_t;

image_t read_image(const char *file_name);

void write_image(image_t image, const char *file_name);

image_t crop(image_t image, int y, int x, int height, int width);
