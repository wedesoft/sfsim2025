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
  for (int k=0; k<6; k++) {
    image_t result = allocate_image(256, 256);
    unsigned char *p = result.data;
    for (int j=0; j<256; j++) {
      for (int i=0; i<256; i++) {
        float x, y, z;
        switch (k) {
          case 0:
            x = -1 + 2 * i / 256.0;
            y = 1;
            z = -1 + 2 * j / 256.0;
            break;
          case 1:
            x = -1 + 2 * i / 256.0;
            y = 1 - 2 * j / 256.0;
            z = 1;
            break;
          case 2:
            x = 1;
            y = 1 - 2 * j / 256.0;
            z = 1 - 2 * i / 256.0;
            break;
          case 3:
            x = 1 - 2 * i / 256.0;
            y = 1 - 2 * j / 256.0;
            z = -1;
            break;
          case 4:
            x = -1;
            y = 1 - 2 * j / 256.0;
            z = -1 + 2 * i / 256.0;
            break;
          case 5:
            x = -1 + 2 * i / 256.0;
            y = -1;
            z = 1 - 2 * j / 256.0;
            break;
        };
        float longitude = atan2(x, z);
        float r = sqrt(x * x + z * z);
        float lattitude = atan(y / r);
        int dx = (int)round(675 * 2 * longitude / M_PI) + 675 * 2;
        int dy = 675 - (int)round(675 * 2 * lattitude / M_PI);
        if (dx >= 4 * 675) dx = 0;
        if (dy >= 2 * 675) dy = 2 * 675 - 1;
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
