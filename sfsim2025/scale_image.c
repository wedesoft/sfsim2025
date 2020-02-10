#include <gc.h>
#include <stdio.h>
#include "sfsim2025/image.h"


int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Syntax: ./scale-image [input image] [output image]\n");
    fprintf(stderr, "Example: ./scale-image world.200404.3x21600x21600.C1.png map2.png\n");
    return 1;
  };
  GC_INIT();
  image_t image = read_image(argv[1]);
  image_t result = scale_image(image);
  write_image(result, argv[2]);
  return 0;
}
