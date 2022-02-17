#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>

int main() {

	// todo: call read_ppm
	int w = 0;
	int h = 0;
	int count = 1;
	char * file = "feep-ascii.ppm";
	struct ppm_pixel* arr = read_ppm(file, &w, &h);
	printf("Testing file %s: %d %d\n", file, w, h);
	for (int i = 0; i < w*h; i++) {
		printf("(%d, %d, %d)", arr[i].red, arr[i].green, arr[i].blue);
		if(count == w) {
			printf("\n");
			count = 0;
		}
		count++;
	}

	free(arr);
	arr = NULL;
	return 0;
}

