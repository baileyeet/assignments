#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include <math.h>
#include <string.h>

int main(int argc, char* argv[]) {
	srand(time(0));
	int size = 480;
	float xmin = -2.0;
	float xmax = 0.47;
	float ymin = -1.12;
	float ymax = 1.12;
	int maxIterations = 1000;

	int opt;
	while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
		switch (opt) {
			case 's': size = atoi(optarg); break;
			case 'l': xmin = atof(optarg); break;
			case 'r': xmax = atof(optarg); break;
			case 't': ymax = atof(optarg); break;
			case 'b': ymin = atof(optarg); break;
			case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
		}
	}
	printf("Generating mandelbrot with size %dx%d\n", size, size);
	printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
	printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

	struct ppm_pixel* base = (struct ppm_pixel*)malloc(maxIterations * sizeof(struct ppm_pixel));
	struct ppm_pixel* palette = (struct ppm_pixel*)malloc(size * size * sizeof(struct ppm_pixel));

	if (base == NULL) {
		printf("Malloc error");
		return -1;
	}

	if (palette == NULL) {
		printf("Malloc error");
		return -1;
	}	

	for (int i = 0; i < maxIterations; i++) {
		base[i].red = rand() % 255;
		base[i].green = rand() % 255;
		base[i].blue = rand() % 255;
	}

	float row = 0;
	float col = 0;
	for (int i = 0; i < size * size; i++) {
		float xfrac = ((float)col)/size;
		float yfrac = ((float)row)/size;
		float x0 = xmin + xfrac * (xmax-xmin);
		float y0 = ymin + yfrac * (ymax-ymin);

		float x,y = 0;
		int iter = 0;

		while (iter < maxIterations && (x*x + y*y) < 2*2) {
			float xtmp = x*x - y*y + x0;
			y = 2*x*y + y0;
			x = xtmp;
			iter++;
		}
		if (iter < maxIterations) { 
			palette[i].red = base[iter].red + rand() % 100 - 50;
			palette[i].blue = base[iter].blue + rand() % 100 - 50;
			palette[i].green = base[iter].green + rand() % 100 - 50;
		}
		else {
			palette[i].red = 0;
			palette[i].blue = 0;
			palette[i].green = 0;
		}
		col ++;

		if (col == size) {
			row++;
			col = 0;
		}

	}


	// todo: your work here
	// generate pallet
	/*char* write = "mandelbrot";
	  strcat(write, "-");
	  strcat(write, size);
	  strcat(write, time(0));
	  strcat(write, ".ppm");
	  printf("Writing file %s\n", write);*/
	write_ppm("example.ppm", palette, size, size);
	free(palette);
	free(base);
	return 0;


}
