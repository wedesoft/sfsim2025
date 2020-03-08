#include <assert.h>
#include <libgen.h>
#include <tgmath.h>
#include "sfsim2025/map.h"
#include "sfsim2025/cache.h"


int main(int argc, char *argv[]) {
  const int in_level = 0;
  const int out_level = 0;
  const int n = pow(2, out_level);
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
            int dx = map_x(lon, w, in_level);
            int dy = map_y(lat, w, in_level);
            int tx = dx / w;
            int ty = dy / w;
            int px = dx % w;
            int py = dy % w;
            image_t source = cache_image(&cache, "world/%d/%d/%d.png", in_level, ty, tx);
            unsigned char *q = source.data + (w * py + px) * 3;
            p[0] = q[0];
            p[1] = q[1];
            p[2] = q[2];
            p += 3;
          };
        };
        mkdir_p(dirname(cubepath("globe", k, out_level, b, a, ".png")));
        write_image(image, cubepath("globe", k, out_level, b, a, ".png"));
      };
    };
  };
  return 0;
}
