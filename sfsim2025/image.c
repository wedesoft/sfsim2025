#include <sys/stat.h>
#include <string.h>
#include <gc.h>
#include <magick/MagickCore.h>
#include "image.h"


// Load image from hard disk.
image_t read_image(const char *file_name) {
  image_t result;
  ExceptionInfo *exception_info = AcquireExceptionInfo();
  ImageInfo *image_info = CloneImageInfo((ImageInfo *)NULL);
  CopyMagickString(image_info->filename, file_name, MaxTextExtent);
  Image *images = ReadImage(image_info, exception_info);
  if (exception_info->severity < ErrorException) {
    Image *image = RemoveFirstImageFromList(&images);
    result.height = image->rows;
    result.width = image->columns;
    result.data = GC_MALLOC_ATOMIC(result.height * result.width * 3);
    ExportImagePixels(image, 0, 0, result.width, result.height, "RGB", CharPixel, result.data, exception_info);
    if (exception_info->severity >= ErrorException) {
      CatchException(exception_info);
      result.height = 0;
      result.width = 0;
      result.data = NULL;
    };
    DestroyImage(image);
    DestroyImageList(images);
  } else {
    result.height = 0;
    result.width = 0;
    result.data = NULL;
  };
  CatchException(exception_info);
  DestroyImageInfo(image_info);
  DestroyExceptionInfo(exception_info);
  return result;
}

image_t crop(image_t image, int y, int x, int height, int width) {
  image_t result;
  result.height = height;
  result.width = width;
  result.data = GC_MALLOC_ATOMIC(result.height * result.width * 3);
  unsigned char *p = result.data;
  unsigned char *q = image.data + y * 3 * image.width + x * 3;
  for (int j=0; j<result.height; j++) {
    memcpy(p, q, result.width * 3);
    p += result.width * 3;
    q += image.width * 3;
  };
  return result;
}

void write_image(image_t image, const char *file_name) {
  ExceptionInfo *exception_info = AcquireExceptionInfo();
  ImageInfo *image_info = AcquireImageInfo();
  GetImageInfo(image_info);
  Image *frame = ConstituteImage(image.width, image.height, "RGB", CharPixel, image.data, exception_info);
  if (exception_info->severity < ErrorException) {
    Image *images = NewImageList();
    AppendImageToList(&images, frame);
    WriteImages(image_info, images, file_name, exception_info);
    CatchException(exception_info);
    DestroyImageList(images);
  };
  CatchException(exception_info);
  DestroyImageInfo(image_info);
  DestroyExceptionInfo(exception_info);
}

// Create a directory and it's parents.
void mkdir_p(const char *path) {
  int i = 1;
  while (i < strlen(path)) {
    if (path[i] == '/') {
      char *prefix = GC_MALLOC_ATOMIC(i + 1);
      strncpy(prefix, path, i);
      prefix[i] = 0;
      mkdir(prefix, 0755);
    };
    i++;
  };
  mkdir(path, 0755);
}

char *tilepath(const char *prefix, int levels, int y, int x) {
  char *result = GC_MALLOC_ATOMIC(4096);
  sprintf(result, "%s", prefix);
  for (int l=levels-1; l>=0; l--) {
    char buf[4];
    int yb = (y >> l) & 1;
    int xb = (x >> l) & 1;
    sprintf(buf, "/%d%d", yb, xb);
    strcat(result, buf);
  };
  strcat(result, ".png");
  return result;
}

image_t scale_image(image_t image) {
  int width = image.width / 2;
  int height = image.height / 2;
  image_t result = (image_t){.width = width, .height = height, .data = GC_MALLOC_ATOMIC(width * height * 3)};
  unsigned char *p0 = result.data;
  unsigned char *q0 = image.data;
  int row = 3 * image.width;
  for (int j=0; j<height; j++) {
    unsigned char *p = p0;
    unsigned char *q = q0;
    for (int i=0; i<width; i++) {
      p[0] = ((int)q[0] + q[3] + q[row    ] + q[row + 3]) / 4;
      p[1] = ((int)q[1] + q[4] + q[row + 1] + q[row + 4]) / 4;
      p[2] = ((int)q[2] + q[5] + q[row + 2] + q[row + 5]) / 4;
      p += 3;
      q += 6;
    };
    p0 += result.width * 3;
    q0 += 2 * image.width * 3;
  };
  return result;
}
