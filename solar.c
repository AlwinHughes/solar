#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "vec.h"
#include "sphere.h"
#include "ray.h"
#include "view.h"
#include "light.h"
#include "pngsetup.h"
#include "renderable.h"
#include "camera.h"

#define ARRLEN(a) (sizeof(a) / sizeof((a)[0]))

unsigned char* scale_to_char(float* arr, size_t size, float scale){

	//printf("scale to char1 %zu\n", sizeof(unsigned char) * size);
	unsigned char* ret = (unsigned char*) malloc(sizeof(unsigned char) * size);
	//printf("scale to char2\n");

	for(size_t i = 0; i < size; i++){
		ret[i] = (unsigned char) (arr[i] * scale * 255);
	}
	return ret;
}


int main(int argc, char* argv[]) {
	//sphere_geom_t sphere = make_sphere_geom(make_vec3d(3,0,0), 1);

	//ray_t ray = make_ray(make_vec3d(0,0,0), make_vec3d(1,0,0));

	int write = 0;
	int view = 1;

	for(int i = 0; i < argc; i++){
		if(strcmp(argv[i], "-w") == 0){
			printf("accepting write\n");
			write = 1;
		}
		if(strcmp(argv[i], "-h") == 0){
			printf("hiding\n");
			view = 0;
		}
	}

	printf("png byte size: %zu\n", sizeof(png_byte));

	int width = 2000;
	int height = 1000;



	FILE * count_fp, * fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep png_row;
	int image_number;
	char file_name[21];

	if(write == 1) {
		pngSetUp(&count_fp, &fp, &png_ptr, &info_ptr, &png_row, &image_number, file_name, width, height);
	}


	/*
	sphere_geom_t sphere = make_sphere_geom(make_vec3d(3,0,0), 1);

	sphere_t spheres[] = { 
		make_sphere(make_sphere_geom(make_vec3d(3,0.4,0), 0.5),
				make_sphere_col(0,1,0) ), 
			make_sphere(make_sphere_geom(make_vec3d(10,0,100), 100),make_sphere_col(1,1,1) ),
			make_sphere(make_sphere_geom(make_vec3d(20,0,0.3), 5),
				make_sphere_col(0,0,1) ),

			make_sphere(make_sphere_geom(make_vec3d(2,0,0), 0.2),
				make_sphere_col(1,0,0) )
	};
	*/

	ray_t test_ray = make_ray(make_vec3d(0,0,0), make_vec3d(10,0,0));


	light_t lights[2];
	//light.pos = make_vec3d(0,0,10);
	lights[0].pos = make_vec3d(0,10,-5);
	lights[0].col = make_vec3f(1,1,1);

	lights[1].pos = make_vec3d(5,-7,-10);
	lights[1].col = make_vec3f(0,0,1);

	renderable_t renderables[4];
	//												pos									radius		color						ambient								 shin, diff
	make_sphere(renderables, make_vec3d(3,0.4,0), 0.5, make_vec3f(0,1,0), make_vec3f(0.1,0.1,0.1), 100, 1);
	make_sphere(renderables +1, make_vec3d(10,0,100), 100, make_vec3f(1,1,1), make_vec3f(0.1,0.1,0.1), 10, 1);
	make_sphere(renderables +2, make_vec3d(20,0,0.3), 5, make_vec3f(0,0,1), make_vec3f(0.1,0.1,0.1), 10, 1);
	make_sphere(renderables +3, make_vec3d(2,0,0), 0.2, make_vec3f(1,0,0), make_vec3f(0.1,0.1,0.1), 10, 1);

	sceene_t sceene;
	make_sceene(ARRLEN(renderables), ARRLEN(lights), lights, renderables, &sceene);
	printf("no lights: %i", ARRLEN(lights));

	ray_t ray;

	print_sceene(&sceene);

	
	printf("\n");

	const int box_width = 200;
	const int box_height = 100; 

	const int num_box_horiz = ceil((float) width / box_width);
	const int num_box_vert = ceil((float) height / box_height);

	float** boxes = (float**) malloc(sizeof(float* ) * num_box_horiz * num_box_vert);
	printf("horiz: %i, vert: %i array length: %i \n", num_box_horiz, num_box_vert, num_box_horiz * num_box_vert);
	printf("box_width: %i, box_height: %i elements per box: %i\n", box_width, box_height, box_width * box_height);

	float * box_start;
	int current_width, current_height;
	vec3f ambient = make_vec3f(0.1,0.1,0.1);
	intersection_t inter;

	vec3d cam_pos = make_vec3d(0,0,0);
	vec3d cam_direc = make_vec3d(1,0.2,0);
	vec3d cam_up = make_vec3d(0,0,1);
	camera_t cam = make_camera(cam_pos, cam_direc, cam_up, width, height);
	float ratio = (float) width / height;

	/*
	for(int i = 0; i < width; i++){
		printf("x %f\n", ratio * ((i / (float) width) - 0.5));
	}
	*/


	//compute intersections by grouping in boxes
	for(int j = 0; j < num_box_vert; j++){
		printf("box v: %i\n", j);
		for(int i = 0; i < num_box_horiz; i++){
			//size_t points_in_box = 
			//printf("box h: %i, box v: %i\n", i, j);
			current_width = fmin(box_width, width - (i * box_width));
			current_height = fmin(box_height, height- (j * box_height));
			//printf("current: box h: %i, box v: %i, index: %i\n", current_width, current_height, i + j * num_box_horiz);

			boxes[i + j * num_box_horiz] = (float*) calloc(3 * current_width * current_height, sizeof(float));
			//printf("3.1\n");

			float* offset = boxes[ i + j * num_box_horiz];
			for(int y = j * box_height; y < current_height + j * box_height; y++){
				for(int x = i * box_width; x < current_width + i * box_width; x++){

					get_ray(&cam, x, y, &ray);

					inter.empty = INTER_EMPTY;
					getClosestInter(&ray, cam_pos, &sceene, &inter);
					if(inter.empty != INTER_EMPTY){
						vec3f v = inter.obj->getColAtInter(inter.obj, &ray, &inter, &sceene, &cam_pos);
						*((vec3f*) offset) = v;
					} 
					inter.empty = INTER_EMPTY;
					/*
					getNearestSphere(ray, spheres, ARRLEN(spheres), cam_pos, &sphere_inter);
					if(sphere_inter.sphere != NULL)
					{

						//getColAtInterSingleL(&sphere_inter, &light, &cam_pos, offset);
						getColAtInterSingleLAmb(&sphere_inter, &light, &ambient, &cam_pos, shinyness, diffuse, offset);

					}
					*/
					offset += 3;
				}

			}
		}
	}

	printf("inters all computed\n");
	current_width = 0;
	current_height = 0;

	//gamma scaling
	float max = -1;
	for(int j = 0; j < num_box_vert; j++){
		for(int i = 0; i < num_box_horiz; i++){
			//size_t points_in_box = 
			current_width = fmin(box_width, width - (i * box_width));
			current_height = fmin(box_height, height- (j * box_height));
			
			float* box_start = boxes[i + j * num_box_horiz];
			for(size_t k = 0; k < current_width * current_height * 3; k++){
				max = fmax(max, box_start[k]);
			}
		}
	}

	printf("found max as %f\n", max);

	unsigned char** char_boxes = (unsigned char**) malloc(sizeof(unsigned char) * num_box_vert * num_box_horiz);

	current_width = 0;
	current_height = 0;

	for(int j = 0; j < num_box_vert; j++){
		current_height = fmin(box_height, height- (j * box_height));

		for(int i = 0; i < num_box_horiz; i++){
			//printf("v: %i, h: %i\n", j, i);

			current_width = fmin(box_width, width - (i * box_width));
			char_boxes[i + num_box_horiz * j] = scale_to_char(boxes[i + j * num_box_horiz], current_width * current_height *  3,  1/max);

		}
	}
	printf("scaled all boxes\n");


	//preview boxes
	
/*	for(int j = 0; j < num_box_vert; j++){
		for(int i = 0; i < num_box_horiz; i++){
			printf("box h: %i, box v: %i\n", i, j);
			current_width = fmin(box_width, width - (i * box_width));

		}
	}
	*/

	if(view)
		view_all(char_boxes, box_width, box_height, width, height, num_box_horiz);

	if(write == 0){
		return 0;
	}

	/*
	for(int row = 0; row < height; row++){
		int box_row = row / box_height;
		int count = 0;
		int row_offset = 0;
		int curr_box_width;

		for(int box_col = 0; box_col < num_box_horiz; box_col++) {
			
			curr_box_width = fmin(box_width, width - box_col * box_width);

			printf("row %i, box_row %i, box_col %i box_array %i \n", row, box_row, box_col, box_row * num_box_horiz+ box_col);

			if((curr_box_width * (box_row % box_height) * sizeof(png_byte) * 3 ) < 0){
				printf("warning size negative\n");
			}
			printf("starting row at %i starting box at %zu \n", row_offset, (curr_box_width * (box_row % box_height) * sizeof(png_byte) * 3 ));


			/*memcpy(
					png_row + row_offset,
					char_boxes[box_row * num_box_horiz+ box_col ] + (curr_box_width * (box_row % box_height) * sizeof(png_byte) * 3 ),
					curr_box_width * sizeof(png_byte) * 3
					);
					/

			
			for(size_t i = 0; i < curr_box_width; i++){
				png_row[i] = char_boxes[box_row * num_box_horiz+ box_col][(curr_box_width * (box_row % box_height) * sizeof(png_byte) * 3 ) + i];
			}
			

			count += curr_box_width * sizeof(png_byte) * 3 ;
			row_offset += curr_box_width;
			printf("copied %zu pixels", curr_box_width * sizeof(png_byte) * 3);
			printf("---\n");
		}
		printf("row %i had %u bytes\n", row, count);
		png_write_row(png_ptr, png_row);
	}
	*/

	int box_x, box_y;
	unsigned char* box;
	int curr_box_width;
	unsigned int index, t1, t2;

	for(int y = 0; y < height; y++ ){
		box_y = y / box_height;
		for(int x = 0; x < width; x++){

			box_x = x / box_width;
			curr_box_width = fmin(box_width, width - box_x * box_width);
			box = char_boxes[box_x + num_box_horiz * box_y];
			index = (curr_box_width * (y % box_height) + (x % box_width)) * sizeof(char) ;
			
			png_row[x* 3 ] = box[index * 3];
			png_row[x* 3 + 1] = box[index * 3 + 1];
			png_row[x* 3 + 2] = box[index * 3 + 2];

		}

		png_write_row(png_ptr, png_row);
	}



		printf("copied all to libpng\n");

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

