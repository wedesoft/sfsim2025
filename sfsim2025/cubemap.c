#include <assert.h>
#include <libgen.h>
#include <tgmath.h>
#include "sfsim2025/map.h"
#include "sfsim2025/cache.h"


float elevation_for_point(cache_t *elevation_cache, int in_level, int width, float x, float y, float z) {
  float lon = longitude(x, y, z);
  float lat = latitude(x, y, z);
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
  float lat = latitude(x, y, z);
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
  const int n = 1 << out_level;
  const int tilesize = 256;
  const int width = 675;
  float radius = 6378000.0;
  cache_t image_cache = make_cache(2 * n * n);
  cache_t elevation_cache = make_cache(2 * n * n);
  for (int k=0; k<6; k++) {
    for (int b=0; b<n; b++) {
      for (int a=0; a<n; a++) {
        image_t image = allocate_image(tilesize, tilesize);
        vertex_tile_t vertex_tile = allocate_vertex_tile(tilesize, tilesize);
        water_t water = allocate_water(tilesize, tilesize);
        unsigned char *p1 = image.data;
        float *p2 = vertex_tile.data;
        unsigned char *p3 = water.data;
        for (int v=0; v<tilesize; v++) {
          float j = cube_coordinate(out_level, tilesize, b, v);
          for (int u=0; u<tilesize; u++) {
            float i = cube_coordinate(out_level, tilesize, a, u);
            float x = cube_map_x(k, j, i);
            float y = cube_map_y(k, j, i);
            float z = cube_map_z(k, j, i);
            color_for_point(&image_cache, in_level, width, x, y, z, p1, p1 + 1, p1 + 2); // output RGB value
            float h = elevation_for_point(&elevation_cache, in_level, width, x, y, z);
            *p3 = h <= 0 ? 255 : 0; // output water mask value
            if (h < 0) h = 0;
            scale_point(x, y, z, radius + h, p2, p2 + 1, p2 + 2); // output 3D coordinate
            p2[3] = u / (float)(tilesize - 1); // output texture coordinate u
            p2[4] = v / (float)(tilesize - 1); // output texture coordinate v
            float dx1, dy1, dz1;
            offset_longitude(x, y, z, in_level, tilesize, &dx1, &dy1, &dz1);
            float dx2, dy2, dz2;
            offset_latitude(x, y, z, in_level, tilesize, &dx2, &dy2, &dz2);
            float sx[3][3] = {{-0.25, 0, 0.25}, {-0.5, 0, 0.5}, {-0.25, 0, 0.25}};
            float sy[3][3] = {{-0.25, -0.5, -0.25}, {0, 0, 0}, {0.25, 0.5, 0.25}};
            vector_t n1 = vector(0, 0, 0);
            vector_t n2 = vector(0, 0, 0);
            for (int dj=-1; dj<=1; dj++) {
              for (int di=-1; di<=1; di++) {
                float xs = x + di * dx1 + dj * dx2;
                float ys = y + di * dy1 + dj * dy2;
                float zs = z + di * dz1 + dj * dz2;
                float hs = elevation_for_point(&elevation_cache, in_level, width, xs, ys, zs);
                float xh, yh, zh;
                scale_point(x, y, z, hs, &xh, &yh, &zh);
                float px = di * dx1 + dj * dx2 + xh;
                float py = di * dy1 + dj * dy2 + yh;
                float pz = di * dz1 + dj * dz2 + zh;
                n1.x += sx[dj + 1][di + 1] * px;
                n1.y += sx[dj + 1][di + 1] * py;
                n1.z += sx[dj + 1][di + 1] * pz;
                n2.x += sy[dj + 1][di + 1] * px;
                n2.y += sy[dj + 1][di + 1] * py;
                n2.z += sy[dj + 1][di + 1] * pz;
              };
            };
            vector_t n = normalize(cross_product(n1, n2));
            p2[5] = n.x;
            p2[6] = n.y;
            p2[7] = n.z;
            p1 += 3;
            p2 += 8;
            p3++;
          };
        };
        mkdir_p(dirname(cubepath("globe", k, out_level, b, a, ".png")));
        write_image(image, cubepath("globe", k, out_level, b, a, ".png"));
        write_vertex_tile(vertex_tile, cubepath("globe", k, out_level, b, a, ".raw"));
        write_water(water, cubepath("globe", k, out_level, b, a, ".bin"));
      };
    };
  };
  return 0;
}
