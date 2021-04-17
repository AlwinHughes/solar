#include "plane.h"

void plane_getClosestInter(const struct renderable* self, const ray_t* ray, intersection_t* out) {
	/*
	 * n . x = d
	 *
	 * r: a + bt
	 *
	 * n . (a + bt) = d
	 * n . a + n . bt = d
	 *
	 * n . bt = d - n . a
	 * t = (d - n. a)/(n . b)
	 *
	 */ 
	vec3d* n = (vec3d*) self->geom_parameters;
	double* d = (double*) (self->geom_parameters + sizeof(vec3d));

	double n_dot_b = dotd(*n, ray->direc);

	if(n_dot_b == 0){
		out->empty = INTER_EMPTY;
		return;
	}

	double n_dot_a = dotd(*n, ray->start);
	float t = (*d - n_dot_a)/n_dot_b;


	if(t <= 0){
		out->empty = INTER_EMPTY;
		return;
	}
	
	out->pos = eval_at_point(*ray, t);
	out->norm = *n;//normalize3d(sub_vec3d(out->pos, *center));
	out->obj = self;
};


void make_plane(renderable_t* r, vec3d norm, double d, vec3f ambient, float shin, float diff){

	r->geom_len = sizeof(vec3d) + sizeof(double);
	r->shade_len = sizeof(vec3f) + sizeof(float);

	void* base = malloc(r->geom_len + r->shade_len);
	
	vec3d* n = (vec3d*) base;
	*n = norm;

	double* dd = (double*) (base + sizeof(vec3d));
	*dd = d;

	vec3f* amb = (vec3f*) (base + r->geom_len);
	*amb = ambient;

	float* s = (float*) (r->geom_len + r->shade_len + sizeof(vec3f));
	*s = shin;

	float* di = (float*) (r->geom_len + r->shade_len + sizeof(vec3f) + sizeof(float));
	*di = diff;

	r->geom_parameters = base;
	r->shader_parameters = (void*) amb;
	r->getColAtInter = &shader_phong;
	r->getClosestInter = &plane_getClosestInter;
};
