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
            int dx = map_x(lon, w, in_level);
            int dy = map_y(lat, w, in_level);
            int tx = dx / w;
            int ty = dy / w;
            int px = dx % w;
            int py = dy % w;
            image_t image_source = cache_image(&image_cache, "world/%d/%d/%d.png", in_level, ty, tx);
            elevation_t elevation_source = cache_elevation(&elevation_cache, "elevation/%d/%d/%d.raw", in_level, ty, tx);
            assert(image_source.width == w);
            assert(image_source.height == w);
            assert(elevation_source.width == w);
            assert(elevation_source.height == w);
            unsigned char *q1 = image_source.data + (w * py + px) * 3;
            short int *q2 = elevation_source.data + w * py + px;
            p1[0] = q1[0];
            p1[1] = q1[1];
            p1[2] = q1[2];
            *p2 = *q2 > 0 ? *q2 : 0;
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
