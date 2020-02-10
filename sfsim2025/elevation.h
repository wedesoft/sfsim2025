#pragma once


typedef struct {
  int height;
  int width;
  short int *data;
} elevation_t;

elevation_t read_elevation(const char *file_name);
