#include <assert.h>
#include <libgen.h>
#include <tgmath.h>
#include "sfsim2025/map.h"
#include "sfsim2025/cache.h"


float elevation_for_point(cache_t *elevation_cache, int in_level, int width, float x, float y, float z) {
  float lon = longitude(x, y, z);
  float lat = lattitude(x, y, z);
  int dx[2], dy[2];
  float xfrac[2], yfrac[2];
  map_pixels_x(lon, width, in_level, &dx[0], &dx[1], &xfrac[0], &xfrac[1]);
  map_pixels_y(lat, width, in_level, &dy[0], &dy[1], &yfrac[0], &yfrac[1]);
  float e[4];
  for (int t=0; t<2; t++) {
    for (int s=0; s<2; s++) {
      int tx = dx[s] / width;
      int ty = dy[t] / width;
      int px = dx[s] % width;
      int py = dy[t] % width;
      elevation_t elevation_source = cache_elevation(elevation_cache, "elevation/%d/%d/%d.raw", in_level, ty, tx);
      assert(elevation_source.width == width);
      assert(elevation_source.height == width);
      short int *q2 = elevation_source.data + width * py + px;
      e[2 * t + s] = *q2 > 0 ? *q2 : 0;
    };
  };
  return e[0] * xfrac[0] * yfrac[0] + e[1] * xfrac[1] * yfrac[0] + e[2] * xfrac[0] * yfrac[1] + e[3] * xfrac[1] * yfrac[1];
}

void color_for_point(cache_t *image_cache, int in_level, int width, float x, float y, float z,
                     unsigned char *red, unsigned char *green, unsigned char *blue) {
  float lon = longitude(x, y, z);
  float lat = lattitude(x, y, z);
  int dx[2], dy[2];
  float xfrac[2], yfrac[2];
  map_pixels_x(lon, width, in_level, &dx[0], &dx[1], &xfrac[0], &xfrac[1]);
  map_pixels_y(lat, width, in_level, &dy[0], &dy[1], &yfrac[0], &yfrac[1]);
  float r[4];
  float g[4];
  float b[4];
  for (int t=0; t<2; t++) {
    for (int s=0; s<2; s++) {
      int tx = dx[s] / width;
      int ty = dy[t] / width;
      int px = dx[s] % width;
      int py = dy[t] % width;
      image_t image_source = cache_image(image_cache, "world/%d/%d/%d.png", in_level, ty, tx);
      assert(image_source.width == width);
      assert(image_source.height == width);
      unsigned char *q1 = image_source.data + (width * py + px) * 3;
      r[2 * t + s] = q1[0];
      g[2 * t + s] = q1[1];
      b[2 * t + s] = q1[2];
    };
  };
  *red   = r[0] * xfrac[0] * yfrac[0] + r[1] * xfrac[1] * yfrac[0] + r[2] * xfrac[0] * yfrac[1] + r[3] * xfrac[1] * yfrac[1];
  *green = g[0] * xfrac[0] * yfrac[0] + g[1] * xfrac[1] * yfrac[0] + g[2] * xfrac[0] * yfrac[1] + g[3] * xfrac[1] * yfrac[1];
  *blue  = b[0] * xfrac[0] * yfrac[0] + b[1] * xfrac[1] * yfrac[0] + b[2] * xfrac[0] * yfrac[1] + b[3] * xfrac[1] * yfrac[1];
}

int main(int argc, char *argv[]) {
  const int in_level = 0;
  const int out_level = 0;
  const int n = pow(2, out_level);
  const int tilesize = 256;
  const int width = 675;
  cache_t image_cache = make_cache(2 * n * n);
  cache_t elevation_cache = make_cache(2 * n * n);
  for (int k=0; k<6; k++) {
    for (int b=0; b<n; b++) {
      for (int a=0; a<n; a++) {
        image_t image = allocate_image(tilesize, tilesize);
        elevation_t elevation = allocate_elevation(tilesize, tilesize);
        unsigned char *p1 = image.data;
        short int *p2 = elevation.data;
        for (int v=0; v<tilesize; v++) {
          float j = cube_coordinate(out_level, tilesize, b, v);
          for (int u=0; u<tilesize; u++) {
            float i = cube_coordinate(out_level, tilesize, a, u);
            float x = cube_map_x(k, j, i);
            float y = cube_map_y(k, j, i);
            float z = cube_map_z(k, j, i);
            color_for_point(&image_cache, in_level, width, x, y, z, p1, p1 + 1, p1 + 2);
            *p2 = elevation_for_point(&elevation_cache, in_level, width, x, y, z);
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
