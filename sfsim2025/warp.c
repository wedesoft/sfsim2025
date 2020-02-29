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
    image_t result = allocate_image(256, 256);
    unsigned char *p = result.data;
    for (int j=0; j<256; j++) {
      for (int i=0; i<256; i++) {
        float x = cube_map_x(k, j / 255.0, i / 255.0);
        float y = cube_map_y(k, j / 255.0, i / 255.0);
        float z = cube_map_z(k, j / 255.0, i / 255.0);
        int dx = (int)round(675 * 2 * longitude(x, y, z) / M_PI) + 675 * 2;
        int dy = 675 - (int)round(675 * 2 * lattitude(x, y, z) / M_PI);
        if (dx >= 4 * 675) dx = 0;
        if (dy >= 2 * 675) dy = 2 * 675 - 1;
        assert(dx >= 0 && dx < 4 * 675);
        assert(dy >= 0 && dy < 2 * 675);
        image_t source = world[dy / 675][dx / 675];
        unsigned char *q = source.data + (675 * (dy % 675) + (dx % 675)) * 3;
        p[0] = q[0];
        p[1] = q[1];
        p[2] = q[2];
        p += 3;
      };
    };
    char buf[128];
    sprintf(buf, "globe%d.png", k);
    write_image(result, buf);
  };
  return 0;
}
