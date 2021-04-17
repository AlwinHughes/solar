#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"
#include "ray.h"

typedef struct {
	vec3d pos, direc, up;
	int h_res, v_res;
} camera_t;

camera_t make_camera(vec3d pos, vec3d direc, vec3d cam_up, int h_res, int v_res);


#endif
