#include <libgen.h>
#include <gc.h>
#include <stdlib.h>
#include <stdio.h>
#include "sfsim2025/image.h"
#include "sfsim2025/elevation.h"


int main(int argc, char *argv[]) {
  if (argc != 5) {
    fprintf(stderr, "Syntax: ./elevationtiles [data] [tilesize] [levels] [prefix]\n");
    fprintf(stderr, "Example: ./elevationtiles elevation.A1.raw 675 4 A1\n");
    return 1;
  };
  GC_INIT();
  int tilesize = atoi(argv[2]);
  int levels = atoi(argv[3]);
  elevation_t elevation = read_elevation(argv[1]);
  for (int j=0; j<elevation.height / tilesize; j++) {
    for (int i=0; i<elevation.width / tilesize; i++) {
      elevation_t tile = crop_elevation(elevation, j * tilesize, i * tilesize, tilesize, tilesize);
      mkdir_p(dirname(tilepath(argv[4], levels, j, i, ".raw")));
      write_elevation(tile, tilepath(argv[4], levels, j, i, ".raw"));
    };
  };
  return 0;
}
