#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
	void *init = sbrk(0);
	void *current;
	printf("The initial top of the heap is %p.\n", init);
	current = sbrk(0);
	int allocated = (int) (current - init);
	printf("The current top of the heap is %p.\n", init);
	printf("Increased by %d (0x%x) bytes.\n", allocated, allocated);
	return 0 ;
}
