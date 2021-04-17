#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"
#include "ray.h"

typedef struct {
	vec3d pos, direc, up, perp;
	int h_res, v_res;
	float ratio;
} camera_t;

camera_t make_camera(vec3d pos, vec3d direc, vec3d cam_up, int h_res, int v_res);

void get_ray(const camera_t* cam, int x, int y, ray_t* out);




#endif
