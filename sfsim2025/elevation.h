#pragma once
#include <gc.h>


typedef struct {
  int height;
  int width;
  short int *data;
} elevation_t;

static inline elevation_t allocate_elevation(int height, int width) {
  return (elevation_t){.height = height, .width = width, .data = GC_MALLOC_ATOMIC(width * height * 2)};
}

elevation_t read_elevation(const char *file_name);

void write_elevation(elevation_t elevation, const char *file_name);

elevation_t scale_elevation(elevation_t elevation);

elevation_t crop_elevation(elevation_t elevation, int y, int x, int height, int width);

typedef struct {
  int height;
  int width;
  unsigned char *data;
} water_t;

static inline water_t allocate_water(int height, int width) {
  return (water_t){.height = height, .width = width, .data = GC_MALLOC_ATOMIC(width * height)};
}

water_t water_from_elevation(elevation_t elevation);
