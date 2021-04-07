#ifndef VEC_HEADER_H
#define VEC_HEADER_H

#include <stdio.h>
#include "math.h"

typedef struct {
	float x,y,z;
} vec3f ;

typedef struct {
	double x,y,z;
} vec3d ;


void print_vec3f(vec3f v);

void print_vec3d(vec3d v);

vec3f add_vec3f(vec3f v, vec3f u);

vec3d add_vec3d(vec3d v, vec3d u);

vec3f sub_vec3f(vec3f v, vec3f u);

vec3d sub_vec3d(vec3d v, vec3d u);

double sizef(vec3f v);

double sized(vec3d v);

float dotf(vec3f v, vec3f u);

double dotd(vec3d v, vec3d u);

vec3d scale(vec3d v, double s);

vec3d make_vec3d(const double x, const double y, const double z);

double dist(const vec3d u, const vec3d v);

#endif
