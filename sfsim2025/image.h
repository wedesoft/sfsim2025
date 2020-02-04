#pragma once


typedef struct {
  int height;
  int width;
  char *data;
} image_t;

image_t read_image(const char *file_name);
