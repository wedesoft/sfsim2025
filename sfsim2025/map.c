#include <assert.h>
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
