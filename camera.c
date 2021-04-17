#include "camera.h"

camera_t make_camera(vec3d pos, vec3d direc, vec3d cam_up, int h_res, int v_res){
	camera_t c;
	c.pos= pos;
	c.direc = direc;
	c.up = cam_up;
	c.h_res = h_res;
	c.v_res = v_res;
	return c;
}


