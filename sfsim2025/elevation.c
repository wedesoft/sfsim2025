#include <gc.h>
#include <tgmath.h>
#include <stdio.h>
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
    result.width = size;
    result.height = size;
    result.data = GC_MALLOC_ATOMIC(n);
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
