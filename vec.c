#include "vec.h"


void print_vec3f(vec3f v){
	printf("(%f,%f,%f)", v.x,v.y,v.z);
};

void print_vec3d(vec3d v){
	printf("(%f,%f,%f)", v.x,v.y,v.z);
};

inline vec3f add_vec3f(vec3f v, vec3f u){
	vec3f ret;
	ret.x = v.x + u.x;
	ret.y = v.y + u.y;
	ret.z = v.z + u.z;
	return ret;
};


inline vec3d add_vec3d(vec3d v, vec3d u){
	vec3d ret;
	ret.x = v.x + u.x;
	ret.y = v.y + u.y;
	ret.z = v.z + u.z;
	return ret;
};


inline vec3f sub_vec3f(vec3f v, vec3f u){
	v.x += u.x;
	v.y += u.y;
	v.z += u.z;
	return v;
};


inline vec3d sub_vec3d(vec3d v, vec3d u){
	v.x -= u.x;
	v.y -= u.y;
	v.z -= u.z;
	return v;
};

inline double sizef(vec3f v) {
	return sqrt(v.x * v.x + v.y*v.y + v.z * v.z);
};

inline double sized(vec3d v) {
	return sqrt(v.x * v.x + v.y*v.y + v.z * v.z);
};


inline float dotf(vec3f v, vec3f u){
	return v.x*u.x + v.y*u.y + v.z*u.z;
};

inline double dotd(vec3d v, vec3d u){
	return v.x*u.x + v.y*u.y + v.z*u.z;
};

inline vec3d scale(vec3d v, double s) {
	v.x *= s;	
	v.y *= s;	
	v.z *= s;	
	return v;
};

inline vec3d make_vec3d(const double x, const double y, const double z){
	vec3d ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}


inline double dist(const vec3d u, const vec3d v){
	return sqrt((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y) + (u.z - v.z) * (u.z - v.z) );
}
