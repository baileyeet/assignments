#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
	void *init = sbrk(0);
	void *current;
	for (int j = 0 ; j < 10; j++) {
		size_t size = 100; // 100 bytes
		int *memory = NULL;
		memory = malloc(size);
		if (memory == NULL) {
			fprintf(stderr, "malloc failed\n");
			return(1);
		} 
		*memory = 123;
		free(memory);
	}
	current = sbrk(0);
	int allocated = (int)((current-init));

	printf("Increased by %d bytes\n", allocated);
	return 0 ;
}
