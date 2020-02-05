#include <libgen.h>
#include <gc.h>
#include <stdlib.h>
#include <stdio.h>
#include "sfsim2025/image.h"


int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Syntax: ./maptiles [image] [prefix]\n");
    fprintf(stderr, "Example: ./maptiles world.200404.3x21600x21600.C2.png dir\n");
    return 1;
  };
  GC_INIT();
  const char *filename = argv[1];
  int tilesize = 100;
  int levels = 2;
  image_t image = read_image(filename);
  for (int j=0; j<image.height / tilesize; j++) {
    for (int i=0; i<image.width / tilesize; i++) {
      image_t tile = crop(image, j * tilesize, i * tilesize, tilesize, tilesize);
      mkdir_p(dirname(tilepath(argv[2], levels, j, i)));
      write_image(tile, tilepath(argv[2], levels, j, i));
    };
  };
  return 0;
}
