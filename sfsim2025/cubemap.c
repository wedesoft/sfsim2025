#include <assert.h>
#include <tgmath.h>
#include "sfsim2025/map.h"
#include "sfsim2025/cache.h"


int main(int argc, char *argv[]) {
  const int level = 0;
  int count = pow(2, level);
  const int n = 1;
  const int size = 256;
  const int w = 675;
  cache_t cache = make_cache(4);
  for (int k=0; k<6;  k++) {
    for (int b=0; b<n; b++) {
      for (int a=0; a<n; a++) {
        image_t image = allocate_image(size, size);
        unsigned char *p = image.data;
        for (int v=0; v<size; v++) {
          float j = ((float)v / (size - 1) + b) / n;
          for (int u=0; u<size; u++) {
            float i = ((float)u / (size - 1) + a) / n;
            float x = cube_map_x(k, j, i);
            float y = cube_map_y(k, j, i);
            float z = cube_map_z(k, j, i);
            float lon = longitude(x, y, z);
            float lat = lattitude(x, y, z);
            int dx = (lon + M_PI) / (2 * M_PI) * w * 4 * count;
            int dy = (M_PI / 2 - lat) / M_PI * w * 2 * count;
            if (dx >= w * 4 * count) dx = 0;
            if (dy >= w * 2 * count) dy = w * 2 * count - 1;
            assert(dx >= 0 && dx < w * 4 * count);
            assert(dy >= 0 && dy < w * 2 * count);
            int tx = dx / w;
            int ty = dy / w;
            int px = dx % w;
            int py = dy % w;
            image_t source = cache_image(&cache, "world/%d/%d/%d.png", level, ty, tx);
            unsigned char *q = source.data + (w * py + px) * 3;
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
    };
  };
  return 0;
}
