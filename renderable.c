#include "renderable.h"


void make_sceene(unsigned int objs_n, unsigned int lights_n, light_t* ls, renderable_t* rs, sceene_t* s){
	s->no_lights = lights_n;
	s->lights = ls;

	s->no_objs = objs_n;
	s->objects = rs;
};


void print_sphere2(const renderable_t* r){
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

void print_sceene(const sceene_t* s){
	printf("Sceene has %i objects, %i lights\n", s->no_objs, s->no_lights);
	printf("lights are:");

	for(int i = 0; i < s->no_lights; i++){
		print_light(s->lights[i]);
	}

	for(int i = 0; i < s->no_objs; i++){
		s->objects[i].print(s->objects +i);
	}


}


void print_intersection(const intersection_t i){
	printf("Intersection at ");
	print_vec3d(i.pos);
	printf(" normal ");
	print_vec3d(i.norm);
	printf(" %i ", i.empty);
	if(i.empty == INTER_NONEMPTY){
		printf(" with ");
		i.obj->print(i.obj);
	} else {
		printf("\n");
	}
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

//vec3f shader_phong(const struct renderable* self, const ray_t* ray, const intersection_t* inter, const struct sceene* sceene, const vec3d* cam) {	
vec3f shader_phong(const renderable_t* self, const ray_t* ray, const intersection_t* inter, const sceene_t* sceene, const vec3d* cam) {	

	vec3f* ambient = (vec3f*)(self->shader_parameters);
	float *shininess, *diff;
	shininess = (float*)(self->shader_parameters + (sizeof(vec3f)));
	diff = (float*)(self->shader_parameters + (sizeof(vec3f)) + sizeof(float));

	vec3f ret = *ambient;

	for(int i = 0; i < sceene->no_lights; i++){

		vec3d inter_to_light = normalize3d(sub_vec3d(sceene->lights[i].pos, inter->pos));
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

		spec = scalef(sceene->lights[i].col, pow(s, *shininess));

		ret = add_vec3f(ret, spec);
	}

	return ret;
};

vec3f shader_phong_shadow(const renderable_t* self, const ray_t* ray, const intersection_t* inter, const sceene_t* sceene, const vec3d* cam) {	

	vec3f* ambient = (vec3f*)(self->shader_parameters);
	float *shininess, *diff;
	shininess = (float*)(self->shader_parameters + (sizeof(vec3f)));
	diff = (float*)(self->shader_parameters + (sizeof(vec3f)) + sizeof(float));

	vec3f ret = *ambient;

	vec3d inter_to_light, inter_to_cam, between;
	float f, s;
	vec3f spec; 

	for(int i = 0; i < sceene->no_lights; i++){

		if(canSeePoint(sceene, inter, &(sceene->lights[i].pos)) == 1){
			//printf("light %i\n",i);

			inter_to_light = normalize3d(sub_vec3d(sceene->lights[i].pos, inter->pos));
			f = fmax(dotd(inter->norm, inter_to_light),0);

			//ret = add_vec3f(scalef(inter->sphere->col, f), ret);

			//diffuse reflection
			ret = add_vec3f(ret, scalef(inter->obj->col, pow(f,*diff)));
			//memcpy(out, &ret, sizeof(float) *3);

			//spectral reflection
			inter_to_cam = normalize3d(sub_vec3d(*cam, inter->pos));
			between = normalize3d(add_vec3d(inter_to_cam, inter_to_light));

			s = fmax(dotd(inter->norm, between),0);

			spec = scalef(sceene->lights[i].col, pow(s, *shininess));

			ret = add_vec3f(ret, spec);
		}
	}

	return ret;
};




void make_sphere2(renderable_t* rend, vec3d v, double r, vec3f col, vec3f ambient, float shininess, float diff){

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
	rend->print = &print_sphere2;
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

	best_dist = 100000000;
	out->empty = INTER_EMPTY;
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

int canSeePoint(const sceene_t* s, const intersection_t* inter, const vec3d* target){
	double dist_to_point = dist(*target, inter->pos);
	ray_t ray;
	ray.start = *target;
	ray.direc = sub_vec3d(inter->pos,*target);
	intersection_t new_inter;
	for(int i = 0; i < s->no_objs; i++){
		if(s->objects + i == inter->obj){
			continue;
		}
		new_inter.empty = INTER_EMPTY;
		s->objects[i].getClosestInter(s->objects+i, &ray, &new_inter);
		if(new_inter.empty != INTER_EMPTY && dist(new_inter.pos, *target) < dist_to_point ) {
			return 0;
		}
	}
	return 1;
}

