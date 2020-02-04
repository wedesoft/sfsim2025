#include <stdio.h>
#include "sfsim2025/image.h"


int main(int argc, char *argv[]) {
  if (argc != 6) {
    fprintf(stderr, "Syntax: ./maptiles [image] [tilesize] [levels] [y-offset] [x-offset]\n");
    fprintf(stderr, "Example: ./maptiles world.200404.3x21600x21600.C2.png 675 6 21600 43200\n");
    return 1;
  };
  const char *filename = argv[1];
  // image_t image = read_image(filename);
  return 0;
}
