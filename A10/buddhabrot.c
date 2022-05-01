#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include "read_ppm.h"
#include <sys/time.h>
#include <string.h>

struct Info {
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
	int *membership;
	int *counter;
	int *maxCount;
	pthread_barrier_t* barrier;
	int id;
};

void generate(struct Info* info) {
	for (int i = info->beginR; i < info->endR; i++) {
		for (int j = info->beginC; j < info->endC; j++) {
			float xfrac = ((float)j)/info->size;
			float yfrac = ((float)i)/info->size;
			float x0 = info->xmin + xfrac * (info->xmax-info->xmin);
			float y0 = info->ymin + yfrac * (info->ymax-info->ymin);

			float x =  0;
			float y = 0;
			int iter = 0;

			while (iter < info->maxIterations && (x*x + y*y) < 2*2) {
				float xtmp = x*x - y*y + x0;
				y = 2*x*y + y0;
				x = xtmp;
				iter++;
			}

			pthread_mutex_lock(info->mutex);
			if (iter < info->maxIterations) { // escaped
				info->membership[(i*(info->size))+j] = 0;
			} else {
				info->membership[(i*(info->size))+j] = 1;
			}

			pthread_mutex_unlock(info->mutex);

		}

	}
	printf("Thread %d finished generate\n", info->id);
}

void step_two(struct Info *info) {
	for (int i = info->beginR; i < info->endR; i++) {
		for (int j = info->beginC; j < info->endC; j++) {
			if (info->membership[(i*info->size)+j]) {
				float xfrac = (float) (j+1)/info->size;
				float yfrac = (float) (i+1)/info->size;
				float x0 = info->xmin + xfrac * (info->xmax - info->xmin);
				float y0 = info->ymin + yfrac * (info->ymax - info->ymin);
				float x = 0;
				float y = 0;
				float xtmp;
				while (x*x + y*y < 2*2) {
					xtmp = x*x - y*y + x0;
					y = 2*x*y + y0;
					x = xtmp;

					int yrow = round(info->size * (y - info->ymin)/(info->ymax - info->ymin));
					int xcol = round(info->size * (x - info->xmin)/(info->xmax - info->xmin));

					if (yrow < 0 || yrow >= info->size) continue; 
					if (xcol < 0 || xcol >= info->size) continue;

					pthread_mutex_lock(info->mutex);
					info->counter[(yrow*info->size)+xcol]++;
					if (info->counter[(yrow*info->size)+xcol] > *(info->maxCount)) {
						*(info->maxCount) = info->counter[(yrow*info->size)+xcol];
					}
					pthread_mutex_unlock(info->mutex);
				}
			}	
		}
	}
	printf("Thread %d finished step 2\n", info->id);
}

void step_three(struct Info *info) {
	float gamma = 0.681;
	float factor = 1.0/gamma;
	for (int i = info->beginC; i < info->endC; i++) {
		for (int j = info->beginR; j < info->endR; j++) {
			float value = 0;
			if (info->counter[(j*info->size)+i] > 0) {
				value = (float)log(info->counter[(j*info->size)+i]) / (float)log(*(info->maxCount));
				value = (float) pow(value, factor);
			}
			struct ppm_pixel color;
			color.red = value * 255;
			color.green = value * 255;
			color.blue = value * 255;

			pthread_mutex_lock(info->mutex);
			info->palette[(j*(info->size))+i] = color;
			pthread_mutex_unlock(info->mutex);

		}
	}
	printf("Thread %d finished step 3i\n", info->id);
}

void run (void* info) {
	struct Info* i = (struct Info*) info;
	generate(i);
	step_two(i);

	pthread_barrier_wait(i->barrier);
	step_three(i);
}

int main(int argc, char* argv[]) {
	int size = 480;
	float xmin = -2.0;
	float xmax = 0.47;
	float ymin = -1.12;
	float ymax = 1.12;
	int maxIterations = 1000;
	int numProcesses = 4;
	pthread_t* threads = malloc(sizeof(pthread_t) * numProcesses);
  if (threads == NULL) {
    printf("Malloc failed\n");
    return -1;
  }
  memset(threads, 0, sizeof(pthread_t) * numProcesses);

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

	memset(palette, 0, maxIterations * sizeof(struct ppm_pixel));

	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);

	int beginR = 0;
	int endR = size/sqrt(numProcesses);
	int beginC = 0;
	int endC = size/sqrt(numProcesses);

	pthread_barrier_t barrier;
	pthread_barrier_init(&barrier, NULL, numProcesses);
	int* membership = malloc(sizeof(int)*size*size);
	if (membership == NULL) {
		printf("Malloc error");
		return -1;
	}
	
	memset(membership, 0, sizeof(int)*size*size);

	int* maxCount = malloc(sizeof(int));
	*maxCount = 0;;
	int* counter = malloc(sizeof(int)*size*size);
	if (counter == NULL) {
		printf("Malloc failed");
		return -1;
	};
	
	memset(counter, 0, sizeof(int)*size*size);

	struct Info* data = malloc(sizeof(struct Info)*numProcesses);
	memset(data, 0, sizeof(struct Info)*numProcesses);

	for (int i = 0; i < numProcesses; i++) {
		int id = i+1;

		struct Info info_chunk = {base, palette, size, xmin, xmax, ymin, ymax, maxIterations, beginR, endR, beginC, endC, &mutex, membership, counter, maxCount, &barrier, id};
		data[i] = info_chunk;
		printf("Thread %d) Sub-image block: cols (%d, %d) to rows (%d,%d)\n", i+1, \
				beginR, endR, beginC, endC);
		pthread_create(&threads[i], NULL, (void*) run, (void*) &data[i]);
		if (i == sqrt(numProcesses)-1) {
			beginR+= size/sqrt(numProcesses);
			endR += size/sqrt(numProcesses);
			beginC = 0;
			endC = size/sqrt(numProcesses);
		} else {
			beginC += size/sqrt(numProcesses);
			endC += size/sqrt(numProcesses);
		}
	}

	for (int i = 0; i < numProcesses; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);

	gettimeofday(&tend, NULL);
        timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
        printf("Computed buddhabrot set (%dx%d) in %.6g seconds\n", size, size,timer);
        char filename[45];
        sprintf(filename, "buddhabrot-%d-%ld.ppm", size, time(0));
        printf("Writing file: %s\n", filename);
        write_ppm(filename, palette, size, size);
        free(palette);
        free(base);
	free(membership);
	free(data);
	free(maxCount);
	free(threads);
	free(counter);
}
