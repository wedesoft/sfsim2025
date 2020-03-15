#pragma once
#include <tgmath.h>

float cube_map_x(int face, float j, float i);

float cube_map_y(int face, float j, float i);

float cube_map_z(int face, float j, float i);

float longitude(float x, float y, float z);

float lattitude(float x, float y, float z);

static float map_x(float longitude, int tilesize, int level) {
  int count = pow(2, level);
  float result = (longitude + M_PI) / (2 * M_PI) * 4 * count * tilesize;
  return result;
}

static float map_y(float lattitude, int tilesize, int level) {
  int count = pow(2, level);
  float result = (M_PI / 2 - lattitude) / M_PI * tilesize * 2 * count;
  return result;
}

static void map_pixels_x(float longitude, int tilesize, int level, int *x0, int *x1, float *frac0, float *frac1) {
  int count = pow(2, level);
  float x = map_x(longitude, tilesize, level);
  *x0 = floor(x);
  *frac1 = x - *x0;
  *frac0 = 1 - *frac1;
  if (*x0 >= 4 * count * tilesize) *x0 = 0;
  *x1 = *x0 + 1;
  if (*x1 >= 4 * count * tilesize) *x1 = 0;
}

static void map_pixels_y(float lattitude, int tilesize, int level, int *y0, int *y1, float *frac0, float *frac1) {
  int count = pow(2, level);
  float y = map_y(lattitude, tilesize, level);
  *y0 = floor(y);
  *frac1 = y - *y0;
  *frac0 = 1 - *frac1;
  if (*y0 >= 2 * count * tilesize) *y0 = 2 * count * tilesize - 1;
  *y1 = *y0 + 1;
  if (*y1 >= 2 * count * tilesize) *y1 = 2 * count * tilesize - 1;
}
