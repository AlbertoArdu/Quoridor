#include "random.h"
static unsigned int next = 1;

/*Numero MAX 32767*/
int rand(void){
	
	next = next * 1103515245 + 12345;
	return ((unsigned)(next/65536)%32768);
}

void srand(unsigned int seed){
	next = seed;
}
