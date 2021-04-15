#ifndef _LIGHT_H
#define _LIGHT_H

#include "vec.h"

typedef struct {
	vec3d pos;
	vec3f col;
} light_t;

void print_light(light_t l);

#endif
