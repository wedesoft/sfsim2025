#include <gc.h>
#include <tgmath.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "elevation.h"


// Load elevation data from file.
elevation_t read_elevation(const char *file_name) {
  elevation_t result;
  struct stat statbuf;
  int error = stat(file_name, &statbuf);
  if (!error) {
    int n = statbuf.st_size;
    int size = (int)round(sqrt(n / 2));
    result = allocate_elevation(size, size);
    FILE *f = fopen(file_name, "rb");
    fread(result.data, 2 * size, size, f);
    fclose(f);
  } else {
    result.width = 0;
    result.height = 0;
    result.data = NULL;
  };
  return result;
}

// Write elevation data to file.
void write_elevation(elevation_t elevation, const char *file_name) {
  FILE *f = fopen(file_name, "wb");
  fwrite(elevation.data, 2 * elevation.width, elevation.height, f);
  fclose(f);
}

// Scale elevation data to 50% of its size.
elevation_t scale_elevation(elevation_t elevation) {
  elevation_t result;
  result.width = elevation.width / 2;
  result.height = elevation.height / 2;
  result.data = GC_MALLOC_ATOMIC(2 * result.width * result.height);
  short int *p0 = result.data;
  short int *q0 = elevation.data;
  for (int j=0; j<result.height; j++) {
    short int *p = p0;
    short int *q = q0;
    for (int i=0; i<result.width; i++) {
      *p = ((int)q[0] + q[1] + q[elevation.width] + q[elevation.width + 1]) / 4;
      p++;
      q += 2;
    };
    p0 += result.width;
    q0 += 2 * elevation.width;
  };
  return result;
}

elevation_t crop_elevation(elevation_t elevation, int y, int x, int height, int width) {
  elevation_t result;
  result.height = height;
  result.width = width;
  result.data = GC_MALLOC_ATOMIC(height * width * 2);
  short int *p = result.data;
  short int *q = elevation.data + y * elevation.width + x;
  for (int j=0; j<result.height; j++) {
    memcpy(p, q, result.width * 2);
    p += result.width;
    q += elevation.width;
  };
  return result;
}
