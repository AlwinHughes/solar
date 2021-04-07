#include "ray.h"

inline ray_t make_ray(const vec3d start, const vec3d direc) {
	ray_t ret;
	ret.start = start;
	ret.direc = direc;
	return ret;
}


void print_ray(const ray_t r) {
	printf("ray: starting ");
	print_vec3d(r.start);
	printf(" directio ");
	print_vec3d(r.direc);
}


inline vec3d eval_at_point(const ray_t ray, double t){
	vec3d temp = ray.direc;
	scale(temp, t);
	return add_vec3d(temp, ray.start);
}
