#include "camera.h"

camera_t make_camera(vec3d pos, vec3d direc, vec3d cam_up, int h_res, int v_res){
	camera_t c;
	c.pos= pos;
	c.direc = normalize3d(direc);
	c.up = normalize3d(cam_up);
	c.h_res = h_res;
	c.v_res = v_res;
	c.ratio = (float) h_res / v_res;
	c.perp = crossd(c.direc, c.up);
	return c;
};

void get_ray(const camera_t* cam, int x, int y, ray_t* out){
	out->start = cam->pos;
	out->direc = cam->direc;

	float h = cam->ratio *( (x / (float) cam->h_res) - 0.5);
	float v = (y / (float) cam->v_res) - 0.5;

	vec3d extra = add_vec3d(scale(cam->perp, h), scale(cam->up, v));

	out->direc = add_vec3d(out->direc, extra);

};
