#include "light.h"

void print_light(light_t l){
	printf("light with col ");
	print_vec3f(l.col);
	printf(" and pos ");
	print_vec3d(l.pos);
	printf("\n");
}
