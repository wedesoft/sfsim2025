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

// Scale 3D coordinates to a given radius.
void scale_point(float x, float y, float z, float radius1, float radius2, float *xs, float *ys, float *zs) {
  float norm = sqrt(x * x + y * y + z * z);
  *xs = x * radius1 / norm;
  *ys = y * radius2 / norm;
  *zs = z * radius1 / norm;
}

// Map coordinate on cube to sphere of given radius.
void spherical_map(int face, float j, float i, float radius1, float radius2, float *x, float *y, float *z) {
  float cube_x = cube_map_x(face, j, i);
  float cube_y = cube_map_y(face, j, i);
  float cube_z = cube_map_z(face, j, i);
  scale_point(cube_x, cube_y, cube_z, radius1, radius2, x, y, z);
}

// Determine 3D center of map tile.
void tile_center(int level, int face, int b, int a, float radius1, float radius2, float *x, float *y, float *z) {
  float j = cube_coordinate(level, 3, b, 1);
  float i = cube_coordinate(level, 3, a, 1);
  spherical_map(face, j, i, radius1, radius2, x, y, z);
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

void offset_latitude(float x, float y, float z, int level, int tilesize, float radius1, float radius2, float *dx, float *dy, float *dz) {
  float lon = longitude(x, y, z);
  float lat = latitude(x, y, z);
  float norm = sqrt(x * x + y * y + z * z);
  vector_t v = vector(0, norm * M_PI / (2 * tilesize * (1 << level)), 0);
  vector_t vs = matrix_vector_dot(rotation_y(-lon), matrix_vector_dot(rotation_z(lat), v));
  *dx = vs.x;
  *dy = vs.y * radius2 / radius1;
  *dz = vs.z;
}
