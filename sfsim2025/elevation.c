#include <gc.h>
#include <tgmath.h>
#include <sys/stat.h>
#include "elevation.h"


// Load elevation data from hard disk.
elevation_t read_elevation(const char *file_name) {
  elevation_t result;
  struct stat statbuf;
  stat(file_name, &statbuf);
  int size = (int)round(sqrt(statbuf.st_size / 2));
  result.width = size;
  result.height = size;
  return result;
}
