#include <tgmath.h>
#include <stdio.h>
#include "sfsim2025/image.h"


int main(int argc, char *argv[]) {
  image_t world[2][4];
  for (int j=0; j<2; j++) {
    for (int i=0; i<4; i++) {
      world[j][i] = read_image(tilepath("world", 2, j, i, ".png"));
    };
  };
  image_t result = allocate_image(256, 256);
  unsigned char *p = result.data;
  for (int j=0; j<256; j++) {
    for (int i=0; i<256; i++) {
      float x, y, z;
      x = -1;
      y = -1 + 2 * i / 256.0;
      z = 1 - 2 * j / 256.0;
      float longitude = atan2(x, z);
      float r = sqrt(x * x + z * z);
      float lattitude = atan(y / r);
      int dx = (int)round(675 * 2 * longitude / M_PI) + 675 * 2;
      int dy = (int)round(675 * 2 * lattitude / M_PI) + 675;
      image_t source = world[dy / 675][dx / 675];
      unsigned char *q = source.data + (675 * (dy % 675) + (dx % 675)) * 3;
      p[0] = q[0];
      p[1] = q[1];
      p[2] = q[2];
      p += 3;
    };
  };
  write_image(result, "test.png");
  return 0;
}
