#include <assert.h>
#include <tgmath.h>
#include <stdio.h>
#include "sfsim2025/image.h"
#include "sfsim2025/elevation.h"
#include "sfsim2025/map.h"


int main(int argc, char *argv[]) {
  image_t world[2][4];
  elevation_t elevation[2][4];
  for (int j=0; j<2; j++) {
    for (int i=0; i<4; i++) {
      world[j][i] = read_image(tilepath("world", 0, j, i, ".png"));
      elevation[j][i] = read_elevation(tilepath("elevation", 0, j, i, ".raw"));
    };
  };
  for (int k=0; k<6; k++) {
    {
      int width = world[0][0].width;
      int height = world[0][0].height;
      image_t image = allocate_image(256, 256);
      unsigned char *p = image.data;
      for (int j=0; j<256; j++) {
        for (int i=0; i<256; i++) {
          float x = cube_map_x(k, j / 255.0, i / 255.0);
          float y = cube_map_y(k, j / 255.0, i / 255.0);
          float z = cube_map_z(k, j / 255.0, i / 255.0);
          int dx = (int)round(width * 2 * longitude(x, y, z) / M_PI) + width * 2;
          int dy = height - (int)round(height * 2 * lattitude(x, y, z) / M_PI);
          if (dx >= 4 * width) dx = 0;
          if (dy >= 2 * height) dy = 2 * height - 1;
          assert(dx >= 0 && dx < 4 * width);
          assert(dy >= 0 && dy < 2 * height);
          image_t source = world[dy / height][dx / width];
          unsigned char *q = source.data + (width * (dy % height) + (dx % width)) * 3;
          p[0] = q[0];
          p[1] = q[1];
          p[2] = q[2];
          p += 3;
        };
      };
      char buf[128];
      sprintf(buf, "globe%d.png", k);
      write_image(image, buf);
    };
    {
      int width = elevation[0][0].width;
      int height = elevation[0][0].height;
      elevation_t image = allocate_elevation(128, 128);
      short int *p = image.data;
      for (int j=0; j<128; j++) {
        for (int i=0; i<128; i++) {
          float x = cube_map_x(k, j / 127.0, i / 127.0);
          float y = cube_map_y(k, j / 127.0, i / 127.0);
          float z = cube_map_z(k, j / 127.0, i / 127.0);
          int dx = (int)round(width * 2 * longitude(x, y, z) / M_PI) + width * 2;
          int dy = height - (int)round(height * 2 * lattitude(x, y, z) / M_PI);
          if (dx >= 4 * width) dx = 0;
          if (dy >= 2 * height) dy = 2 * height - 1;
          elevation_t source = elevation[dy / height][dx / width];
          *p = *(source.data + width * (dy % height) + (dx % width));
          p++;
        };
      };
      char buf[128];
      sprintf(buf, "globe%d.raw", k);
      write_elevation(image, buf);
    };
  };
  return 0;
}
