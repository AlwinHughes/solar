#ifndef SPHERE_HEADER_H
#define SPHERE_HEADER_H
#include "vec.h"
#include "ray.h"
#include "light.h"

typedef struct {
	vec3d center;
	double radius;
} sphere_geom_t;

typedef vec3f sphere_col_t;

typedef struct {
	sphere_geom_t geom;
	sphere_col_t col;
} sphere_t;

typedef struct {
	sphere_t * sphere;
	vec3d pos;
	vec3d norm;
} sphere_inter_t;

typedef struct {
	float a,b,c, disc;
} inter_first_t;

typedef struct {
	vec3d pos;
} inter_second_t;

sphere_geom_t make_sphere_geom(const vec3d center, const double radius);

sphere_t make_sphere(const sphere_geom_t geom, sphere_col_t col);

int does_intersect(const sphere_geom_t s, const ray_t r);

float does_intersect2(const sphere_geom_t s, const ray_t r);

vec3d get_pos_inter(const sphere_geom_t s, const ray_t r);

void print_sphere(const sphere_geom_t s);

void first_half_compute_inter(const sphere_geom_t s, const ray_t r, inter_first_t * inter);

void second_half_compute_inter(const ray_t r, inter_first_t * inter, inter_second_t * inter_res);
void getNearestSphere(const ray_t r, sphere_t spheres[], size_t num_spheres, const vec3d cam_pos, sphere_inter_t * inter);

sphere_col_t make_sphere_col(float r, float g, float b);

void print_full_sphere(const sphere_t s);

void print_sphere_col(const sphere_col_t col);


#endif
