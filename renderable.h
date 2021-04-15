#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <stdlib.h>
#include <math.h>

#include "vec.h"
#include "ray.h"
#include "light.h"
#include "sceene.h"

typedef enum {
	INTER_EMPTY = 0, 
	INTER_NONEMPTY = 1
} intersection_type_t ;

typedef struct intersection {
	intersection_type_t empty;
	vec3d pos, norm;
	struct renderable* obj;
} intersection_t;

void print_intersection(const intersection_t);


typedef struct renderable {
	
	vec3f(*preview) (const ray_t, const struct sceene*);

	vec3f(*getColAtInter) (const struct renderable* self, const ray_t*, const intersection_t*, const struct sceene*, const vec3d* cam);

	void (*getClosestInter) (const struct renderable* self, const ray_t*, intersection_t* out);
	//int (*canSeePoint)(const void* renderable, const vec3d, const vec3d pos);
	void (*print)(const struct renderable* self);
	void* geom_parameters, *shader_parameters;
	vec3f col;
	size_t geom_len, shade_len;

} renderable_t;


typedef struct sceene {

	struct renderable* objects;
	unsigned int no_lights, no_objs;
	light_t* lights;
	//void (*getClosestInter) (const ray_t, const vec3d cam_pos);

} sceene_t;

void free_renderable(const renderable_t* r);

void getClosestInter(const ray_t* ray, const vec3d cam_pos, const sceene_t* sceene, intersection_t* out);


vec3f shader_phong(const struct renderable* self, const ray_t* ray, const intersection_t* inter, const struct sceene* sceene, const vec3d* cam);

vec3f shader_phong_shadow(const renderable_t* self, const ray_t* ray, const intersection_t* inter, const sceene_t* sceene, const vec3d* cam);

void make_sphere2(renderable_t* r, vec3d pos, double radius, vec3f col, vec3f ambient, float shin, float diff);

void make_plane(renderable_t* r, vec3d, double, vec3f ambient, float shin, float diff);

void make_sceene(unsigned int objs_n, unsigned int lights_n, light_t* ls, renderable_t* rs, sceene_t* s);

void print_sceene(const sceene_t* s);

int canSeePoint(const sceene_t* s, const intersection_t* inter, const vec3d* target);


enum RENDERABLE_TYPE {
	SPHERE,
	PLANE
};

#endif
