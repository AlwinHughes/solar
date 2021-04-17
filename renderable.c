#include "renderable.h"


void make_sceene(unsigned int objs_n, unsigned int lights_n, light_t* ls, renderable_t* rs, sceene_t* s){
	s->no_lights = lights_n;
	s->lights = ls;

	s->no_objs = objs_n;
	s->objects = rs;
};



void print_sceene(const sceene_t* s){
	printf("Sceene has %i objects, %i lights\n", s->no_objs, s->no_lights);
	printf("lights are:");

	for(int i = 0; i < s->no_lights; i++){
		print_light(s->lights[i]);
	}

	for(int i = 0; i < s->no_objs; i++){
		if(s->objects[i].print)
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

			inter_to_light = normalize3d(sub_vec3d(sceene->lights[i].pos, inter->pos));
			f = fmax(dotd(inter->norm, inter_to_light),0);

			//diffuse reflection
			ret = add_vec3f(ret, scalef(inter->obj->col, pow(f,*diff)));

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


void free_renderable(const renderable_t* r) {
	free(r->geom_parameters);
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

