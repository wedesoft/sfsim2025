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
void spherical_map(int k, float j, float i, float radius, float *x, float *y, float *z) {
  float cube_x = cube_map_x(k, j, i);
  float cube_y = cube_map_y(k, j, i);
  float cube_z = cube_map_z(k, j, i);
  float distance = sqrt(cube_x * cube_x + cube_y * cube_y + cube_z * cube_z);
  float factor = radius / distance;
  *x = cube_x * factor;
  *y = cube_y * factor;
  *z = cube_z * factor;
}

// Determine 3D center of map tile.
void tile_center(int level, int k, int b, int a, float radius, float *x, float *y, float *z) {
  float j = cube_coordinate(level, 3, b, 1);
  float i = cube_coordinate(level, 3, a, 1);
  spherical_map(k, j, i, radius, x, y, z);
}
