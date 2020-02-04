#include <gc.h>
#include <magick/MagickCore.h>
#include "image.h"


image_t read_image(const char *file_name) {
  image_t result;
  ExceptionInfo *exception_info = AcquireExceptionInfo();
  ImageInfo *image_info = CloneImageInfo((ImageInfo *)NULL);
  CopyMagickString(image_info->filename, file_name, MaxTextExtent);
  Image *images = ReadImage(image_info, exception_info);
  if (exception_info->severity < ErrorException) {
    CatchException(exception_info);
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
  } else {
    CatchException(exception_info);
    result.height = 0;
    result.width = 0;
    result.data = NULL;
  };
  DestroyImageInfo(image_info);
  DestroyExceptionInfo(exception_info);
  return result;
}
