#include "renderable.h"


void make_sceene(unsigned int objs_n, unsigned int lights_n, light_t* ls, renderable_t* rs, sceene_t* s){

	s->no_lights = lights_n;
	s->lights = ls;

	s->no_objs = objs_n;
	s->objects = rs;
};

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
	
	out->pos = eval_at_point(*r, t);
	out->norm = normalize3d(sub_vec3d(out->pos, *center));
	out->obj = self;
};

//vec3f shader_phong(const struct renderable* self, const ray_t* ray, const intersection_t* inter, const struct sceene* sceene, const vec3d* cam) {	
vec3f shader_phong(const renderable_t* self, const ray_t* ray, const intersection_t* inter, const sceene_t* sceene, const vec3d* cam) {	

	vec3f* ambient = (vec3f*)(self->shader_parameters);
	float *shininess, *diff;
	shininess = (float*)(self->shader_parameters + (sizeof(vec3f)));
	diff = (float*)(self->shader_parameters + (sizeof(vec3f)) + sizeof(float));

	vec3f ret = *ambient;

	vec3d inter_to_light = normalize3d(sub_vec3d(sceene->lights[0].pos, inter->pos));
	float f = fmax(dotd(inter->norm, inter_to_light),0);

	//ret = add_vec3f(scalef(inter->sphere->col, f), ret);
	
	//diffuse reflection
	ret = scalef(inter->obj->col, pow(f,*diff));
	//memcpy(out, &ret, sizeof(float) *3);

	//spectral reflection
	vec3f spec; 
	vec3d inter_to_cam = normalize3d(sub_vec3d(*cam, inter->pos));
	vec3d between = normalize3d(add_vec3d(inter_to_cam, inter_to_light));

	float s = fmax(dotd(inter->norm, between),0);

	spec = scalef(sceene->lights[0].col, pow(s, *shininess));
	
	ret = add_vec3f(ret, spec);

	return ret;
};

void make_sphere2(renderable_t* rend, vec3d v, double r, vec3f ambient, float shininess, float diff){

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

	
	rend->geom_parameters =  base;
	rend->shader_parameters = (void*) amb;
	rend->getClosestInter = &sphere_getClosestInter;
	rend->getColAtInter = &shader_phong;
};

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


void free_renderable(const renderable_t* r) {
	free(r->geom_parameters);
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

void getClosestInter(const ray_t* ray, const vec3d cam_pos, const sceene_t* sceene, intersection_t* out){

	intersection_t next_inter;
	float next_dist, best_dist;

	best_dist = -1;
	for(int i = 0; i < sceene->no_objs; i++){
		sceene->objects[i].getClosestInter(sceene->objects + i, ray, &next_inter);

		if(next_inter.empty == INTER_EMPTY){
			continue;
		}
	next_dist = dist(next_inter.pos, cam_pos);

		if(next_dist < best_dist){
			best_dist = next_dist;
			*out= next_inter;
		}
	}
};

