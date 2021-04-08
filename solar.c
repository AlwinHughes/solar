#include <stdio.h>
#include <png.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
//#include <immintrin.h>

#include "vec.h"
#include "sphere.h"
#include "ray.h"
#include "view.h"
#include "phong.h"
#include "light.h"


char* scale_to_char(float* arr, size_t size, float scale){

	char* ret = (char*) malloc(sizeof(char) * size);

	for(size_t i = 0; i < size; i++){
		ret[i] = (char) (arr[i] * scale * 255);
	}

	return ret;
}


int main(int argc, char* argv[]) {
	//sphere_geom_t sphere = make_sphere_geom(make_vec3d(3,0,0), 1);

	//ray_t ray = make_ray(make_vec3d(0,0,0), make_vec3d(1,0,0));

	int write = 0;

	for(int i = 0; i < argc; i++){
		if(strcmp(argv[i], "-w") == 0){
			printf("accepting write\n");
			write = 1;
		}
	}

	int width = 800;
	int height = 800;


	vec3d cam_pos = make_vec3d(0,0,0);


	char* line = (char* ) malloc(10);
	size_t line_length;
	FILE * count_fp, * fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep png_row;
	int image_number;
	char file_name[21];

	if(write == 1) {

		printf("i'm going to do a png\n");

		count_fp = fopen("count", "r+");

		if(!count_fp){
			printf("Couldn't open count file\n");
			return 1;
		}

		printf("1\n");

		getline(&line, &line_length,count_fp);

		printf("2\n");

		image_number = atoi(line);
		image_number++;
		//char image_num_str[20];
		sprintf(file_name,"images/image_%i.png", image_number);

		if(access(file_name, F_OK) == 0) {
			fprintf(stderr, "image file already exists\n");
			return 1;
		}


		fp = fopen(file_name, "wb");
		if(!fp){
			fprintf(stderr, "couldn't write to file");
			return 1;
		}

		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		if(!png_ptr){
			fprintf(stderr, "couldn't allocated write structure 1\n");
		}

		info_ptr = png_create_info_struct(png_ptr);
		if(!info_ptr){
			fprintf(stderr, "couldn't allocated write structure 2\n");
		}

		png_init_io(png_ptr, fp);

		png_set_IHDR(png_ptr, info_ptr, width, height,
				8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		png_write_info(png_ptr, info_ptr);

		png_row = (png_bytep) malloc(3 * width * sizeof(png_byte));
	}

	int res;
	float ratio = (float) width / height;



	inter_first_t inter_1;
	inter_second_t inter_2;

	float t;

	sphere_geom_t sphere = make_sphere_geom(make_vec3d(3,0,0), 1);

	size_t num_sphere = 2;
	sphere_t spheres[] = { 
		make_sphere(make_sphere_geom(make_vec3d(3,0.4,0), 1),
				make_sphere_col(1,1,0) ), 
		//make_sphere(make_sphere_geom(make_vec3d(4,1,0), 2),make_sphere_col(1,0,0) ),
		make_sphere(make_sphere_geom(make_vec3d(4,0,0.3), 2),
				make_sphere_col(0.5,0,1) ) 
	};

	light_t light;
	//light.pos = make_vec3d(0,0,10);
	light.pos = make_vec3d(0,0,1);
	light.col = make_vec3f(1,1,1);

	ray_t ray;

	sphere_inter_t sphere_inter;
	
	printf("\n");

	const size_t box_width = 150;
	const size_t box_height = 149; 

	const size_t num_box_horiz = ceil((float) width / box_width);
	const size_t num_box_vert = ceil((float) height / box_height);

	float** boxes = (float**) malloc(sizeof(float* ) * num_box_horiz * num_box_vert);
	printf("horiz: %i, vert: %i array length: %i \n", num_box_horiz, num_box_vert, num_box_horiz * num_box_vert);
	printf("box_width: %i, box_height: %i elements per box: %i\n", box_width, box_height, box_width * box_height);

	float * box_start;
	int current_width, current_height;

	//compute intersections by grouping in boxes
	for(int j = 0; j < num_box_vert; j++){
		for(int i = 0; i < num_box_horiz; i++){
			//size_t points_in_box = 
			printf("box h: %i, box v: %i\n", i, j);
			current_width = fmin(box_width, width - (i * box_width));
			current_height = fmin(box_height, height- (j * box_height));
			//printf("current: box h: %i, box v: %i, index: %i\n", current_width, current_height, i + j * num_box_horiz);

			boxes[i + j * num_box_horiz] = (float*) calloc(3 * current_width * current_height, sizeof(float));
			//printf("3.1\n");

			float* offset = boxes[ i + j * num_box_horiz];
			for(int y = j * box_height; y < current_height + j * box_height; y++){
				for(int x = i * box_width; x < current_width + i * box_width; x++){
					//printf("x: %i, y:%i\n", x,y);

					ray = make_ray( cam_pos, make_vec3d(1, ratio * (x / (float) width) - 0.5,   (y / (float) height) - 0.5 ));
					//printf("3.2\n");
					getNearestSphere(ray, spheres, num_sphere, cam_pos, &sphere_inter);
					//printf("3.3\n");
					//printf("copy form #%i\n", i+j * num_box_vert);
					if(sphere_inter.sphere != NULL)
					{

						getColAtInterSingleL(&sphere_inter, &light, &cam_pos, offset);

						//memcpy(offset, (void *) &sphere_inter.sphere->col, sizeof(float) * 3);
						//printf("inter in box %i %i\n", i, j);
						//printf("#");
					}
					//memcpy(boxes[ i + j * num_box_horiz]+(i * current_width + j), (void *) &sphere_inter.sphere->col, sizeof(float) * 3);
					//printf("3.4\n");
					offset += 3;
				}
				//printf("\n");

			}
			//printf("3.9----\n");
		}
	}

	printf("inters all computed\n");

	//gamma scaling
	float max = 1;
	for(int i = 0; i < num_box_horiz; i++){
		for(int j = 0; j < num_box_vert; j++){
			//size_t points_in_box = 
			size_t current_width = fmin(box_width, width - (i * num_box_horiz));
			size_t current_height = fmin(box_height, height- (i * num_box_vert));
			
			float* box_start = boxes[i + j * num_box_horiz];
			for(size_t k = 0; k < current_width * current_height; k++){
				max = fmax(max, sqrt(box_start[k]));
			}
		}
	}

	printf("found max as %f\n", max);

	char** char_boxes = (char**) malloc(sizeof(char) * num_box_vert * num_box_horiz);

	current_width = 0;
	current_height = 0;

	for(int j = 0; j < num_box_vert; j++){
		current_height = fmin(box_height, height- (j * box_height));

		for(int i = 0; i < num_box_horiz; i++){

			current_width = fmin(box_width, width - (i * box_width));
			char_boxes[i + num_box_horiz * j] = scale_to_char(boxes[i + j * num_box_horiz], current_width * current_height *  3,  1/max);

		}
	}


	//preview boxes
	
	for(int j = 0; j < num_box_vert; j++){
		for(int i = 0; i < num_box_horiz; i++){
			printf("box h: %i, box v: %i\n", i, j);
			current_width = fmin(box_width, width - (i * box_width));
			current_height = fmin(box_height, height- (j * box_height));
			//view_single(char_boxes[i + num_box_horiz * j], current_width, current_height);
		}
	}

	view_all(char_boxes, box_width, box_height, width, height, num_box_horiz);


	printf("scaled all boxes\n");

	/*
	for(int y = 0; y< height; y++){
		for(int x = 0; x < width; x++){
			ray = make_ray( cam_pos, make_vec3d(1, ratio * (x / (float) width) - 0.5,   (y / (float) height) - 0.5 ));
			
			getNearestSphere(ray, spheres, num_sphere, cam_pos, &sphere_inter);
			//first_half_compute_inter(sphere, ray, &inter_1);

			//t = does_intersect2(sphere, ray);

			if(sphere_inter.sphere != NULL) {
				row[x*3] = sphere_inter.sphere->col.r;
				row[x*3 + 1] = sphere_inter.sphere->col.g;
				row[x*3 + 2] = sphere_inter.sphere->col.b;
			} else {
				row[x*3] = 0;
				row[x*3 + 1] = 0;
				row[x*3 + 2] = 0;
			}
		}
		if(y % 25 == 0)
			printf("row %i\n", y);
		png_write_row(png_ptr, row);
	}
	*/

	if(write == 0){
		return 0;
	}

	for(size_t row = 0; row < height;row++){
		size_t box_row = floor((float) row / box_height);
		size_t count = 0;
		size_t row_offset = 0;

		for(size_t box_col  = 0; box_col < num_box_horiz; box_col++) {


			//memcpy(row + (box_width * sizeof(float) * 3), boxes[row * num_box_vert + i], 3 * sizeof(float) );

			size_t curr_box_width = fmin(box_width, width - box_col * box_width);
			//row_offset = box_col * box_width * 3;

			/** if(curr_box_width == box_width) { */
			/**   block_offset =  */
			//printf("row: %u col: %u index: %u\n", box_row, box_col, box_row * num_box_horiz+ box_col);
			//printf("block_offset: %i , row_offset: %i, copying: %i\n", block_offset, row_offset, curr_box_width);
			//printf("width - block_offset: %i\n", width - block_offset);
			/**  */
			/**   memcpy(png_row + (row_offset* sizeof(png_byte) * 3), boxes[box_row * num_box_horiz+ box_col ] + sizeof(png_byte) * 3 * block_offset, curr_box_width * sizeof(png_byte) * 3); */
			/**   printf("---\n"); */
			/**  */
			/** } else { */
			memcpy(
					png_row + (row_offset * sizeof(png_byte) * 3),
					char_boxes[box_row * num_box_horiz+ box_col ] + (curr_box_width * (box_row % box_height) * sizeof(png_byte) * 3 ),
					curr_box_width * sizeof(png_byte) * 3
					);
			count += curr_box_width * sizeof(png_byte);
			row_offset += curr_box_width;
			printf("copied %i pixels", curr_box_width * sizeof(png_byte));
			printf("---\n");
		}
		printf("row %i had %u elements\n", row, count);
		png_write_row(png_ptr, png_row);
	}

		printf("copied all to libpng");

	png_write_end(png_ptr, NULL);

	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (png_row != NULL) free(png_row);



	/*
	for(int j = -height; j < height; j++) {
		for(int i = -width; i < width; i++) {

			ray = make_ray(cam_pos, make_vec3d(1, j / (float) height, i / (float) width));
			int res = does_intersect(sphere, ray);


		}
	}

	
	for(int j = -height; j < height; j++) {
		for(int i = -width; i < width; i++) {
			ray = make_ray(cam_pos, make_vec3d(1, j / (float) height, i / (float) width));

			int res = does_intersect(sphere, ray);
			
			if(res){
				printf("#");
			} else {
				printf(" ");
			}
		}
			printf("\n");
	}
	*/


	//update count file
	
	rewind(count_fp);
	fprintf(count_fp, "%i", image_number);
	printf("%s", file_name);
	fclose(count_fp);

	return 0;
};

