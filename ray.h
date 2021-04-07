#ifndef RAY_HEADER_H
#define RAY_HEADER_H
#include "vec.h"

typedef struct {
	vec3d start;
	vec3d direc;
} ray_t;

ray_t make_ray(const vec3d start, const vec3d direc);

void print_ray(const ray_t r);

vec3d eval_at_point(const ray_t ray, double t);

#endif
