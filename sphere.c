#include "sphere.h"
#include "vec.h"
#include <math.h>

void print_sphere(const renderable_t* r){
	vec3d* center = (vec3d*) r->geom_parameters;
	double* radius = (double*) (r->geom_parameters + sizeof(vec3d));

	vec3f* ambient = (vec3f*)(r->shader_parameters);
	float *shininess, *diff;
	shininess = (float*)(r->shader_parameters + (sizeof(vec3f)));
	diff = (float*)(r->shader_parameters + (sizeof(vec3f)) + sizeof(float));

	printf("Sphere: center at ");
	print_vec3d(*center);
	printf(" radius %f", *radius);
	printf(" color ");
	print_vec3f(r->col);
	printf(" ambient ");
	print_vec3f(*ambient);
	printf(" shiny %f diff %f\n", *shininess, *diff);
}

void sphere_getClosestInter(const struct renderable* self, const ray_t* r, intersection_t* out){
	vec3d* center = (vec3d*) self->geom_parameters;
	double* radius = (double*) (self->geom_parameters + sizeof(vec3d));

	double a = dotd(r->direc, r->direc);
	vec3d start_min_center = sub_vec3d(r->start, *center);
	double b = 2.0 * dotd(start_min_center, r->direc);
	double c = dotd(start_min_center, start_min_center) - (*radius * *radius);

	float disc_sq = b * b - 4.0 * a * c;
	if(disc_sq < 0){
		out->empty = INTER_EMPTY;
		return;
	} 

	float t = (b > 0)*(-b + sqrt(disc_sq))/ (2.0 * a) + (b <= 0) *(-b - sqrt(disc_sq))/ (2.0 * a); 

	if(t <= 0){
		out->empty = INTER_EMPTY;
		return;
	}
	
	out->empty = INTER_NONEMPTY;
	out->pos = eval_at_point(*r, t);
	out->norm = normalize3d(sub_vec3d(out->pos, *center));
	out->obj = self;
};


void make_sphere(renderable_t* rend, vec3d v, double r, vec3f col, vec3f ambient, float shininess, float diff){

	rend->geom_len = sizeof(vec3d) + sizeof(double);
	rend->shade_len = sizeof(vec3f) + sizeof(float);
	//									 center                 radius 	 ambient					shininess and diff
	void * base  = malloc(rend->geom_len + rend->shade_len);

	vec3d* center = (vec3d*) base;
	*center = v;

	double* radius = (double*) (base + sizeof(vec3d));
	*radius = r;

	vec3f* amb = (vec3f*) (base + rend->geom_len);
	*amb = ambient;

	float* shi = (float*) (base + rend->geom_len + sizeof(vec3f));
	*shi = shininess;

	float* di = (float*) (base + rend->geom_len + sizeof(vec3f) + sizeof(float));
	*di = diff;

	rend->col = col;
	rend->geom_parameters =  base;
	rend->shader_parameters = (void*) amb;
	rend->getClosestInter = &sphere_getClosestInter;
	rend->getColAtInter = &shader_phong_shadow;
	rend->print = &print_sphere;
};
