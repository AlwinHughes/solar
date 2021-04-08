#ifndef _PHONG_H
#define _PHONG_H

#include <string.h>

#include "vec.h"
#include "ray.h"
#include "sphere.h"
#include "light.h"


void getColAtInterSingleL(sphere_inter_t* inter, light_t* light, vec3d* cam, void* out);


#endif
