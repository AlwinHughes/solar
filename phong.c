#include "phong.h"


void getColAtInterSingleL(sphere_inter_t* inter, light_t* light, vec3d* cam, void* out){

	vec3f ret;

	//is light visible
	vec3d inter_to_light = normalize3d(sub_vec3d(light->pos, inter->pos));
	float f = dotd(inter->norm, inter_to_light);
	if(f <= 0){
		memcpy(out, &ret, sizeof(float) *3);
		return;
	}

	//ret = scalef(ele_mul_3f(inter->sphere->col,light->col), f);
	ret = scalef(inter->sphere->col, f);

	memcpy(out, &ret, sizeof(float) *3);
}

