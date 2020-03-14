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
  cache_t image_cache = make_cache(4);
  cache_t elevation_cache = make_cache(4);
  for (int k=0; k<6;  k++) {
    for (int b=0; b<n; b++) {
      for (int a=0; a<n; a++) {
        image_t image = allocate_image(size, size);
        elevation_t elevation = allocate_elevation(size, size);
        unsigned char *p1 = image.data;
        short int *p2 = elevation.data;
        for (int v=0; v<size; v++) {
          float j = ((float)v / (size - 1) + b) / n;
          for (int u=0; u<size; u++) {
            float i = ((float)u / (size - 1) + a) / n;
            float x = cube_map_x(k, j, i);
            float y = cube_map_y(k, j, i);
            float z = cube_map_z(k, j, i);
            float lon = longitude(x, y, z);
            float lat = lattitude(x, y, z);
            int dx[2], dy[2];
            float xfrac, yfrac;
            map_pixels_x(lon, w, in_level, &dx[0], &dx[1], &xfrac);
            map_pixels_y(lat, w, in_level, &dy[0], &dy[1], &yfrac);
            float r[4];
            float g[4];
            float b[4];
            float e[4];
            for (int t=0; t<2; t++) {
              for (int s=0; s<2; s++) {
                int tx = dx[s] / w;
                int ty = dy[t] / w;
                int px = dx[s] % w;
                int py = dy[t] % w;
                image_t image_source = cache_image(&image_cache, "world/%d/%d/%d.png", in_level, ty, tx);
                elevation_t elevation_source = cache_elevation(&elevation_cache, "elevation/%d/%d/%d.raw", in_level, ty, tx);
                assert(image_source.width == w);
                assert(image_source.height == w);
                assert(elevation_source.width == w);
                assert(elevation_source.height == w);
                unsigned char *q1 = image_source.data + (w * py + px) * 3;
                short int *q2 = elevation_source.data + w * py + px;
                r[2 * t + s] = q1[0];
                g[2 * t + s] = q1[1];
                b[2 * t + s] = q1[2];
                e[2 * t + s] = *q2 > 0 ? *q2 : 0;
              };
            };
            p1[0] = r[0] * (1 - xfrac) * (1 - yfrac) + r[1] * xfrac * (1 - yfrac) + r[2] * (1 - xfrac) * yfrac + r[3] * xfrac * yfrac;
            p1[1] = g[0] * (1 - xfrac) * (1 - yfrac) + g[1] * xfrac * (1 - yfrac) + g[2] * (1 - xfrac) * yfrac + g[3] * xfrac * yfrac;
            p1[2] = b[0] * (1 - xfrac) * (1 - yfrac) + b[1] * xfrac * (1 - yfrac) + b[2] * (1 - xfrac) * yfrac + b[3] * xfrac * yfrac;
            *p2 = e[0] * (1 - xfrac) * (1 - yfrac) + e[1] * xfrac * (1 - yfrac) + e[2] * (1 - xfrac) * yfrac + e[3] * xfrac * yfrac;
            p1 += 3;
            p2++;
          };
        };
        mkdir_p(dirname(cubepath("globe", k, out_level, b, a, ".png")));
        write_image(image, cubepath("globe", k, out_level, b, a, ".png"));
        mkdir_p(dirname(cubepath("globe", k, out_level, b, a, ".raw")));
        write_elevation(elevation, cubepath("globe", k, out_level, b, a, ".raw"));
      };
    };
  };
  return 0;
}
