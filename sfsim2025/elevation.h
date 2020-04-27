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

water_t read_water(const char *file_name);

void write_water(water_t water, const char *file_name);

typedef struct {
  int height;
  int width;
  float *data;
} vertex_tile_t;

static inline vertex_tile_t allocate_vertex_tile(int height, int width) {
  return (vertex_tile_t){.height = height, .width = width, .data = GC_MALLOC_ATOMIC(width * height * 8 * sizeof(float))};
}

vertex_tile_t read_vertex_tile(const char *file_name);

void write_vertex_tile(vertex_tile_t vertices, const char *file_name);
