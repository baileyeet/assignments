#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"

void generate(struct ppm_pixel* base, struct ppm_pixel* palette, int size, float xmin, float xmax, float ymin, float ymax, int maxIterations, int beginR, int endR, int beginC, int endC) {

	float row = beginR;
	float col = beginC;
	for (int r = beginR; r < endR; r++) {
		for (int c = beginC; c < endC; c++) {
		int i = size*r + c; 
		float xfrac = ((float)col)/size;
		float yfrac = ((float)row)/size;
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

		if (col == endC) {
			row++;
			col = 0;
		}

		}
	}

}

int main(int argc, char* argv[]) {
	int size = 2000;
	float xmin = -2.0;
	float xmax = 0.47;
	float ymin = -1.12;
	float ymax = 1.12;
	int maxIterations = 1000;
	int numProcesses = 4;
	pid_t pid, mypid1, mypid2, mypid3, mypid4;

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
	struct ppm_pixel* palette;

	if (base == NULL) {
		printf("Malloc error");
		return -1;
	}

	int shmid;                                                                     
	shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) *size*size, 0644 | IPC_CREAT);            
	if (shmid == -1) {                                                             
		perror("Error: cannot initialize shared memory\n");                          
		exit(1);                                                                     
	}                                                                              

	palette = shmat(shmid, NULL, 0);                                          
	if (palette == (void*) -1) {                                                    
		perror("Error: cannot access shared memory\n");                              
		exit(1);                                                                     
	}     

	for (int i = 0; i < maxIterations; i++) {
		base[i].red = rand() % 255;
		base[i].green = rand() % 255;
		base[i].blue = rand() % 255;
	}

	for(int count = 1; count <= 4;count++){
		pid = fork();
		if(pid == 0){
			if(count == 1){
				printf("Lauched child process: %d\n",getpid());
				printf("%d) Sub-image block: cols(%d, %d) to rows (%d, %d)\n", getpid(), 0, size/2, 0, size/2);
				generate(base, palette, size, xmin, xmax, ymin, ymax, maxIterations, 0, size/2, 0, size/2);
				fflush(stdout);
				printf("Child process complete: %d\n", getpid());
				exit(0);
			} else if(count == 2){
				printf("Launched child process: %d\n",getpid());
				printf("%d) Sub-image block: cols(%d, %d) to rows (%d, %d)\n", getpid(), size/2, size, 0, size/2);
				generate(base, palette, size, xmin, xmax, ymin, ymax, maxIterations, size/2, size, 0, size/2);
				printf("Child process complete: %d\n", getpid());
				fflush(stdout);
				exit(0);
			} else if(count == 3){
				printf("Launched child process: %d\n",getpid());
				printf("%d) Sub-image block: cols(%d, %d) to rows (%d, %d)\n", getpid(), 0, size/2, size/2, size);
				generate(base, palette, size, xmin, xmax, ymin, ymax, maxIterations, 0, size/2, size/2, size);
				printf("Child process complete: %d\n", getpid());
				fflush(stdout);
				exit(0);
			} else if(count == 4){
				printf("Launched child process: %d\n",getpid());
				printf("%d) Sub-image block: cols(%d, %d) to rows (%d, %d)\n", getpid(), size/2, size, size/2, size);
				generate(base, palette, size, xmin, xmax, ymin, ymax, maxIterations, size/2, size, size/2, size);
				fflush(stdout);
				printf("Child process complete: %d\n", getpid());
				exit(0);
			}
		}
		else{
			fflush(stdout);
		}
	}

	for (int i = 1; i <=4; i++) {
		int status;
		int pid = wait(&status);
		fflush(stdout);
	}

	gettimeofday(&tend, NULL);
        timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
        printf("Computed mandelbrot set (%dx%d) in %.6g seconds\n", size, size,timer);
        char filename[45];
        sprintf(filename, "multi-mandelbrot-%d-%ld.ppm", size, time(0));
        printf("Writing file: %s\n", filename);
        write_ppm(filename, palette, size, size);

	
	free(base);
	if (shmdt(palette) == -1) {                                                     
		perror("Error: cannot detatch from shared memory\n");                        
		exit(1);                                                                     
	}                                                                              

	if (shmctl(shmid, IPC_RMID, 0) == -1) {                                        
		perror("Error: cannot remove shared memory\n");                              
		exit(1);                                                                     
	}          
}
