#include <assert.h>
#include <gc.h>
#include <tgmath.h>
#include <stdbool.h>
#include "map.h"
#include "vector.h"
#include "matrix.h"


float cube_map_x(int face, float j, float i) {
  float result;
  switch (face) {
    case 0:
      result = -1 + 2 * i;
      break;
    case 1:
      result = -1 + 2 * i;
      break;
    case 2:
      result = 1;
      break;
    case 3:
      result = 1 - 2 * i;
      break;
    case 4:
      result = -1;
      break;
    case 5:
      result = -1 + 2 * i;
      break;
    default:
      assert(false);
  };
  return result;
}

float cube_map_y(int face, float j, float i) {
  float result;
  switch (face) {
    case 0:
      result = 1;
      break;
    case 1:
      result = 1 - 2 * j;
      break;
    case 2:
      result = 1 - 2 * j;
      break;
    case 3:
      result = 1 - 2 * j;
      break;
    case 4:
      result = 1 - 2 * j;
      break;
    case 5:
      result = -1;
      break;
    default:
      assert(false);
  };
  return result;
}

float cube_map_z(int face, float j, float i) {
  float result;
  switch (face) {
    case 0:
      result = -1 + 2 * j;
      break;
    case 1:
      result = 1;
      break;
    case 2:
      result = 1 - 2 * i;
      break;
    case 3:
      result = -1;
      break;
    case 4:
      result = -1 + 2 * i;
      break;
    case 5:
      result = 1 - 2 * j;
      break;
    default:
      assert(false);
  };
  return result;
}

int *cube_indices(int size) {
  int *result = GC_MALLOC_ATOMIC((size - 1) * (size - 1) * 2 * 3 * sizeof(int));
  int *p = result;
  for (int j=0; j<size - 1; j++) {
    for (int i=0; i<size - 1; i++) {
      *p++ = j * size + i + 1;
      *p++ = j * size + i;
      *p++ = (j + 1) * size + i;
      *p++ = (j + 1) * size + i;
      *p++ = (j + 1) * size + i + 1;
      *p++ = j * size + i + 1;
    };
  };
  return result;
}

// Map coordinate on cube to sphere of given radius.
void spherical_map(int face, float j, float i, float radius, float *x, float *y, float *z) {
  float cube_x = cube_map_x(face, j, i);
  float cube_y = cube_map_y(face, j, i);
  float cube_z = cube_map_z(face, j, i);
  float distance = sqrt(cube_x * cube_x + cube_y * cube_y + cube_z * cube_z);
  float factor = radius / distance;
  *x = cube_x * factor;
  *y = cube_y * factor;
  *z = cube_z * factor;
}

// Determine 3D center of map tile.
void tile_center(int level, int face, int b, int a, float radius, float *x, float *y, float *z) {
  float j = cube_coordinate(level, 3, b, 1);
  float i = cube_coordinate(level, 3, a, 1);
  spherical_map(face, j, i, radius, x, y, z);
}

// Compute 3D vertices for part of a sphere using elevation data.
vertex_tile_t cube_vertices(elevation_t elevation, float radius, int face, int level, int b, int a) {
  int width = elevation.width;
  int height = elevation.height;
  vertex_tile_t vertices = allocate_vertex_tile(width, height);
  float *p = vertices.data;
  short int *e = elevation.data;
  assert(e);
  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      float cube_j = cube_coordinate(level, height, b, j);
      float cube_i = cube_coordinate(level, width, a, i);
      int h = *e > 0 ? *e : 0;
      spherical_map(face, cube_j, cube_i, radius + h, p, p + 1, p + 2);
      p += 3;
      p[0] = i / (float)(width - 1);
      p[1] = j / (float)(height - 1);
      p += 2;
      e++;
    };
  };
  return vertices;
}

void offset_longitude(float x, float y, float z, int level, int tilesize, float *dx, float *dy, float *dz) {
  float lon = longitude(x, y, z);
  float norm = sqrt(x * x + y * y + z * z);
  vector_t v = vector(0, 0, -norm * M_PI / (2 * tilesize * (1 << level)));
  vector_t vs = matrix_vector_dot(rotation_y(-lon), v);
  *dx = vs.x;
  *dy = vs.y;
  *dz = vs.z;
}

void offset_latitude(float x, float y, float z, int level, int tilesize, float *dx, float *dy, float *dz) {
  float lon = longitude(x, y, z);
  float lat = latitude(x, y, z);
  float norm = sqrt(x * x + y * y + z * z);
  vector_t v = vector(0, norm * M_PI / (2 * tilesize * (1 << level)), 0);
  vector_t vs = matrix_vector_dot(rotation_y(-lon), matrix_vector_dot(rotation_z(lat), v));
  *dx = vs.x;
  *dy = vs.y;
  *dz = vs.z;
}
