#ifndef PLANE_HEADER_H
#define PLANE_HEADER_H
#include "vec.h"
#include "ray.h"
#include "light.h"
#include "renderable.h"

void make_plane(renderable_t* r, vec3d norm, double d, vec3f ambient, float shin, float diff);


void plane_getClosestInter(const struct renderable* self, const ray_t* ray, intersection_t* out);

#endif
