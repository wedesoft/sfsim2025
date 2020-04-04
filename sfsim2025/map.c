#include <assert.h>
#include <gc.h>
#include <tgmath.h>
#include <stdbool.h>
#include "map.h"


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

float longitude(float x, float y, float z) {
  return atan2(x, z);
}

float lattitude(float x, float y, float z) {
  float r = sqrt(x * x + z * z);
  return atan2(y, r);
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
float *cube_vertices(elevation_t elevation, float radius, int face, int level, int b, int a) {
  int width = elevation.width;
  int height = elevation.height;
  float *vertices = GC_MALLOC_ATOMIC(width * height * 5 * sizeof(float));
  float *p = vertices;
  short int *e = elevation.data;
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
