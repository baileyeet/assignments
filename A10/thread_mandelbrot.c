#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <pthread.h>
#include "read_ppm.h"

struct Info {
	int id;
	struct ppm_pixel* base;
	struct ppm_pixel* palette;
	int size;
	float xmin;
	float xmax;
	float ymin;
	float ymax;
	int maxIterations;
	int beginR;
	int endR;
	int beginC;
	int endC;
	pthread_mutex_t* mutex;
};

void generate(int id, struct ppm_pixel* base, struct ppm_pixel* palette, int size, float xmin, float xmax, float ymin, float ymax, int maxIterations, int beginR, int endR, int beginC, int endC, pthread_mutex_t* mutex) {
	float row = beginR;
	float col = beginC;
	for (int r = beginR; r < endR; r++) {
		for (int c = beginC; c < endC; c++) {
			int i = size*r + c;
			float xfrac = ((float)c)/size;
			float yfrac = ((float)r)/size;
			float x0 = xmin + xfrac * (xmax-xmin);
			float y0 = ymin + yfrac * (ymax-ymin);

			float x =  0;
			float y = 0;
			int iter = 0;

			while (iter < maxIterations && (x*x + y*y) < 2*2) {
				float xtmp = x*x - y*y + x0;
				y = 2*x*y + y0;
				x = xtmp;
				iter++;
			}
			if (iter < maxIterations) {
				pthread_mutex_lock(mutex);
				palette[i].red = base[iter].red + rand() % 100 - 50;
				palette[i].blue = base[iter].blue + rand() % 100 - 50;
				palette[i].green = base[iter].green + rand() % 100 - 50;
				pthread_mutex_unlock(mutex);
			}
			else {
				palette[i].red = 0;
				palette[i].blue = 0;
				palette[i].green = 0;
			}
			col ++;

			if (col == endC) {
				row++;
				col = 0;
			}

		}
	}
	printf("Thread %d) finished\n", id);
}

void Compute(void *id) {
	struct Info *myid = (struct Info *) id;
	generate(myid->id, myid->base, myid->palette, myid->size, myid->xmin, myid->xmax, myid->ymin, myid->ymax, myid->maxIterations, myid->beginR, myid->endR, myid->beginC, myid->endC, myid->mutex);

}

int main(int argc, char* argv[]) {
	int size = 480;
	float xmin = -2.0;
	float xmax = 0.47;
	float ymin = -1.12;
	float ymax = 1.12;
	int maxIterations = 1000;
	int numProcesses = 4;

	int opt;
	while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
		switch (opt) {
			case 's': size = atoi(optarg); break;
			case 'l': xmin = atof(optarg); break;
			case 'r': xmax = atof(optarg); break;
			case 't': ymax = atof(optarg); break;
			case 'b': ymin = atof(optarg); break;
			case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
						  "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
		}
	}

	printf("Generating mandelbrot with size %dx%d\n", size, size);
	printf("  Num processes = %d\n", numProcesses);
	printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
	printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;

	struct timeval tstart, tend;
	double timer;
	srand(time(0));
	gettimeofday(&tstart, NULL);

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

	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	struct Info one = {1, base, palette, size, xmin, xmax, ymin, ymax, maxIterations, 0, size/2, 0, size/2, &mutex};
	pthread_create(&thread1, NULL, (void*) Compute, (void*) &one);
	printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d,%d)\n", one.id, 0, size/2, 0, size/2);
	struct Info two = {2, base, palette, size, xmin, xmax, ymin, ymax, maxIterations, size/2, size, 0, size/2, &mutex};
	pthread_create(&thread2, NULL, (void*) Compute, (void*) &two);
	printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d,%d)\n", two.id, size, size, 0, size/2);
	struct Info three = {3, base, palette, size, xmin, xmax, ymin, ymax, maxIterations, 0, size/2, size/2, size, &mutex};
	pthread_create(&thread3, NULL, (void*) Compute, (void*) &three);
	printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d,%d)\n", three.id, 0, size/2, size/2, size);
	struct Info four = {4, base, palette, size, xmin, xmax, ymin, ymax, maxIterations, size/2, size, size/2, size, &mutex};
	pthread_create(&thread4, NULL, (void*) Compute, (void*) &four);
	printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d,%d)\n", four.id, size/2, size, size/2, size);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);

	gettimeofday(&tend, NULL);
	timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
	printf("Computed mandelbrot set (%dx%d) in %.6g seconds\n", size, size,timer);
	char filename[45];
	sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
	printf("Writing file: %s\n", filename);
	write_ppm(filename, palette, size, size);
	free(palette);
	free(base);
	// todo: your code here
	// generate pallet
	// compute image
}
