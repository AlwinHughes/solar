#include <stdio.h>
#include <png.h>
#include <stdlib.h>
#include <unistd.h>

void pngSetUp(FILE** count_fp, FILE** fp, png_structp* png_ptr, png_infop* into_ptr, png_bytep* png_row ,int* image_number, char file_name[21], int width, int height);
