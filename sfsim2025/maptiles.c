#include <libgen.h>
#include <gc.h>
#include <stdlib.h>
#include <stdio.h>
#include "sfsim2025/image.h"


int main(int argc, char *argv[]) {
  if (argc != 7) {
    fprintf(stderr, "Syntax: ./maptiles [image] [tilesize] [level] [prefix] [y offset] [x offset]\n");
    fprintf(stderr, "Example: ./maptiles world.200404.3x21600x21600.C1.png 675 5 C1 0 2\n");
    return 1;
  };
  GC_INIT();
  int tilesize = atoi(argv[2]);
  int levels = atoi(argv[3]);
  int dy = atoi(argv[5]) << levels;
  int dx = atoi(argv[6]) << levels;
  image_t image = read_image(argv[1]);
  for (int j=0; j<image.height / tilesize; j++) {
    for (int i=0; i<image.width / tilesize; i++) {
      image_t tile = crop_image(image, j * tilesize, i * tilesize, tilesize, tilesize);
      mkdir_p(dirname(tilepath(argv[4], levels, j + dy, i + dx, ".png")));
      write_image(tile, tilepath(argv[4], levels, j + dy, i + dx, ".png"));
    };
  };
  return 0;
}
