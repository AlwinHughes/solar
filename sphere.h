#ifndef SPHERE_HEADER_H
#define SPHERE_HEADER_H
#include "vec.h"
#include "ray.h"
#include "light.h"
#include "renderable.h"

void make_sphere(renderable_t* r, vec3d pos, double radius, vec3f col, vec3f ambient, float shin, float diff);

void print_sphere(const renderable_t* r);

void sphere_getClosestInter(const struct renderable* self, const ray_t* r, intersection_t* out);
#endif
