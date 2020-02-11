#include <gc.h>
#include <stdio.h>
#include "sfsim2025/elevation.h"


int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Syntax: ./scale-elevation [input data] [output data]\n");
    fprintf(stderr, "Example: ./scale-elevation elevation.A1.raw elevation2.raw\n");
    return 1;
  };
  GC_INIT();
  elevation_t elevation = read_elevation(argv[1]);
  elevation_t result = scale_elevation(elevation);
  write_elevation(result, argv[2]);
  return 0;
}
