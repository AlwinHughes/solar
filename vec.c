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


inline vec3f scalef(vec3f v, float s) {
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

inline vec3f make_vec3f(const float x, const float y, const float z){
	vec3f ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}


inline double dist(const vec3d u, const vec3d v){
	return sqrt((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y) + (u.z - v.z) * (u.z - v.z) );
}


vec3d ele_mul_3d(vec3d v, vec3d u){
	vec3d ret;
	ret.x = v.x * u.y;
	ret.x = v.y * u.y;
	ret.x = v.z * u.z;
	return ret;
}


vec3f ele_mul_3f(vec3f v, vec3f u){
	vec3f ret;
	ret.x = v.x * u.y;
	ret.x = v.y * u.y;
	ret.x = v.z * u.z;
	return ret;
}


vec3d normalize3d(vec3d v){
	double s = 1/sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return scale(v, s);
}
