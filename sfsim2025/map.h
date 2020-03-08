#pragma once
#include <tgmath.h>

float cube_map_x(int face, float j, float i);

float cube_map_y(int face, float j, float i);

float cube_map_z(int face, float j, float i);

float longitude(float x, float y, float z);

float lattitude(float x, float y, float z);

static int map_x(float longitude, int tilesize, int level) {
  int count = pow(2, level);
  int result = (longitude + M_PI) / (2 * M_PI) * 4 * count * tilesize;
  if (result >= 4 * count * tilesize) result = 0;
  return result;
}

static int map_y(float lattitude, int tilesize, int level) {
  int count = pow(2, level);
  int result = (M_PI / 2 - lattitude) / M_PI * tilesize * 2 * count;
  if (result >= 2 * count * tilesize) result = 2 * count * tilesize - 1;
  return result;
}
