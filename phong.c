#include "phong.h"

void getColAtInterSingleL(sphere_inter_t* inter, light_t* light, vec3d* cam, void* out){

	vec3f ret = make_vec3f(0,0,0);

	//is light visible
	vec3d inter_to_light = normalize3d(sub_vec3d(light->pos, inter->pos));
	float f = dotd(inter->norm, inter_to_light);
	if(f <= 0){
		printf("wrong side\n");
		memcpy(out, &ret, sizeof(float) *3);
		return;
	}

	
	//printf("norm: %f\n",sized(inter->norm));

	//printf("f %f\n", f);

	//ret = scalef(ele_mul_3f(inter->sphere->col,light->col), f);
	ret = add_vec3f(ret, scalef(inter->sphere->col, f));

	memcpy(out, &ret, sizeof(float) *3);
}

//   getColAtInterSingleLAmb(&sphere_inter 				, &light				, &ambient 			, &cam_pos  , diffuse 			 , shinyness,  offset);
void getColAtInterSingleLAmb(sphere_inter_t* inter, light_t* light, vec3f* ambient, vec3d* cam, float shininess, float diff, void* out){

	vec3f ret = *ambient;

	//is light visible
	vec3d inter_to_light = normalize3d(sub_vec3d(light->pos, inter->pos));
	float f = fmax(dotd(inter->norm, inter_to_light),0);

	//ret = add_vec3f(scalef(inter->sphere->col, f), ret);
	
	//diffuse reflection
	ret = scalef(inter->sphere->col, pow(f,diff));
	//memcpy(out, &ret, sizeof(float) *3);

	//spectral reflection
	vec3f spec; 
	vec3d inter_to_cam = normalize3d(sub_vec3d(*cam, inter->pos));
	vec3d between = normalize3d(add_vec3d(inter_to_cam, inter_to_light));

	float s = fmax(dotd(inter->norm, between),0);

	spec = scalef(light->col, pow(s, shininess));
	
	ret = add_vec3f(ret, spec);

	memcpy(out, &ret, sizeof(float) *3);

}
