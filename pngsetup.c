#include "pngsetup.h"


void pngSetUp(FILE** count_fp, FILE** fp, png_structp* png_ptr, png_infop* info_ptr, png_bytep* png_row,int* image_number, char file_name[21], int width, int height){
	size_t line_length;
	char* line = malloc(10);

		printf("i'm going to do a png\n");

		*count_fp = fopen("count", "r+");

		if(!*count_fp){
			printf("Couldn't open count file\n");
			exit(1);
		}

		printf("1\n");

		getline(&line, &line_length,*count_fp);

		printf("2\n");

		*image_number = atoi(line);
		(*image_number)++;
		free(line);
		//char image_num_str[20];
		sprintf(file_name,"images/image_%i.png", *image_number);

		if(access(file_name, F_OK) == 0) {
			fprintf(stderr, "image file %s already exists\n", file_name);
			exit(1);
		}


		*fp = fopen(file_name, "wb");
		if(!*fp){
			fprintf(stderr, "couldn't write to file");
			exit(1);
		}

		*png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		if(!*png_ptr){
			fprintf(stderr, "couldn't allocated write structure 1\n");
		}

		*info_ptr = png_create_info_struct(*png_ptr);
		if(!*info_ptr){
			fprintf(stderr, "couldn't allocated write structure 2\n");
		}

		png_init_io(*png_ptr, *fp);

		png_set_IHDR(*png_ptr, *info_ptr, width, height,
				8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		png_write_info(*png_ptr, *info_ptr);

		*png_row = (png_bytep) malloc(3 * width * sizeof(png_byte));
}
