#include "sphere.h"
#include "vec.h"
#include <math.h>

int does_intersect(const sphere_geom_t s, const ray_t r){
/* r: a + bt
 * s: |x - c| = f 
 * | (a + bt) - c | = f
 * intersection if | (a - bt) - c| < f for some t
 * 
 * solve |(a - bt - c)|^2 = f^2 for t
 * 
 *
 */

	//a b c now refer to the quadratic coeficients
	double a = dotd(r.direc, r.direc);
	vec3d start_min_center =  sub_vec3d(r.start, s.center);
	double b = 2.0 * dotd(start_min_center, r.direc);
	double c = dotd(start_min_center, start_min_center) - (s.radius * s.radius);

	//printf("a: %f, b: %f, c: %f\n", a,b,c);

	return b * b - 4.0 * a * c > 0;
};

vec3d get_pos_inter(const sphere_geom_t s, const ray_t r){
/* r: a + bt
 * s: |x - c| = f 
 * | (a + bt) - c | = f
 * intersection if | (a - bt) - c| < f for some t
 * 
 * solve |(a - bt - c)|^2 = f^2 for t
 * 
 *
 */

	//a b c now refer to the quadratic coeficients
	double a = dotd(r.direc, r.direc);
	vec3d start_min_center = sub_vec3d(r.start, s.center);
	double b = 2.0 * dotd(start_min_center, r.direc);
	double c = dotd(start_min_center, start_min_center) - (s.radius * s.radius);

	//printf("disc: %f", b * b - 4.0f * a * c);

	double sqrt_disc = sqrt(b * b - 4.0 * a * c);

	double t = (-b + sqrt_disc)/ (2.0 * a);

	vec3d ret = scale(r.direc, t);
	ret = add_vec3d(ret, r.start);
	return ret;
};

void print_sphere(const sphere_geom_t s) {
	//printf("SphereG center: ");
	print_vec3d(s.center);
	printf(" with radius %f", s.radius);
}

void print_full_sphere(const sphere_t s){
	printf("print full sphere unimplimented");	
}

void print_sphere_col(const sphere_col_t col){
	printf("r: %i, g: %i, b: %i", col.x, col.y, col.z);
}

sphere_geom_t make_sphere_geom(const vec3d center, const double radius){
	sphere_geom_t ret;
	ret.center = center;
	ret.radius = radius;
	return ret;
}


void first_half_compute_inter(const sphere_geom_t s, const ray_t r, inter_first_t * inter){

	double a = dotd(r.direc, r.direc);
	vec3d start_min_center =  sub_vec3d(r.start, s.center);
	double b = 2.0 * dotd(start_min_center, r.direc);
	double c = dotd(start_min_center, start_min_center) - (s.radius * s.radius);

	inter->a = a;
	inter->b = b;
	inter->c = c;
	inter->disc = b * b - 4.0 * a * c;
}


void second_half_compute_inter(const ray_t r, inter_first_t * inter, inter_second_t * inter_res){

	double t = (-inter->b + sqrt(inter->disc)) / 2.0 * inter->a;
	inter_res->pos = scale(r.direc, t);
	inter_res->pos = add_vec3d(inter_res->pos, r.start);

}


float does_intersect2(const sphere_geom_t s, const ray_t r){

	double a = dotd(r.direc, r.direc);
	vec3d start_min_center =  sub_vec3d(r.start, s.center);
	double b = 2.0 * dotd(start_min_center, r.direc);
	double c = dotd(start_min_center, start_min_center) - (s.radius * s.radius);

	float disc_sq = b * b - 4.0 * a * c;
	if(disc_sq < 0){
		return -1.0;
	} 

	return (-b + sqrt(disc_sq))/ (2.0 * a);
}

void getNorm(sphere_inter_t* inter) {
	inter->norm = sub_vec3d(inter->pos, inter->sphere->geom.center);
	inter->norm = normalize3d(inter->norm);
};


void getNearestSphere(const ray_t r, sphere_t spheres[], size_t num_spheres, const vec3d cam_pos, sphere_inter_t * inter){

	float best_dist, curr_dist, t, best_t;
	curr_dist = 0;
	best_dist = 10000000;

	inter->sphere = NULL;

	for(size_t i = 0; i < num_spheres; i++){
		t = does_intersect2(spheres[i].geom, r);
		if(t < 0){
			continue;
		} 

		curr_dist = dist(cam_pos,eval_at_point(r, t));

		if(curr_dist < best_dist){
			best_dist = curr_dist;
			best_t = t;
			inter->sphere = &spheres[i];
		}
	}

	if(inter->sphere){
	//	printf("got an inter with norm\n");
		inter->pos = eval_at_point(r, best_t);
		getNorm(inter);
	//	print_vec3d(inter->norm);
	}
}

sphere_t make_sphere(const sphere_geom_t geom, sphere_col_t col) {
	sphere_t s;
	s.col = col;
	s.geom = geom;
	return s;
}

sphere_col_t make_sphere_col(float r, float g, float b) {
	sphere_col_t col;
	col.x = r;
	col.y = g;
	col.z = b;
	return col;
}
