#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
		if (argc < 2) {
			printf("Need param");
			return -1;
		}
	char* file = argv[1];	
	int w = 0;
	int h = 0;
	struct ppm_pixel* pxs = read_ppm(file, &w, &h);
	printf("Reading %s with a width %d and a height %d\n", file, w, h);
	for (int i = 0; i < w * h; i++) {
		pxs[i].red = pxs[i].red << (rand() % 2);
		pxs[i].green = pxs[i].green << (rand() % 2);
		pxs[i].blue = pxs[i].blue << (rand() % 2);
	}
	char* write = strtok(file, ".");
	
	strcat(write, "-glitch.ppm");
	printf("Writing file %s\n", write);
	write_ppm(write, pxs, w, h);
	return 0;
}
