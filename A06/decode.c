#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
	int w = 0;
	int h = 0;
	int count = 0;
	unsigned char str = 0;
	if (argc < 2) {
		printf("Need input file");
			return -1;
	}
	char * file = argv[1];
	struct ppm_pixel* arr = read_ppm(file, &w, &h);
	printf("Testing file %s with width %d and height %d\n", file, w, h);
	printf("Max number of characters in image: %d\n", (w*h*3)/8);
	for (int i = 0; i < w*h; i++) {
		for (int j = 0; j < 3; j++) {
			if (count < 8) {
				unsigned char val = arr[i].colors[j] % 2;
				str = str | val << (7 - count);
				count++;
			}
			if (count == 8) {
				printf("%c", str);
				count = 0;
				str = 0;
			}
		}
	}
	printf("\n");
	free(arr);
	arr = NULL;
	return 0;
}

