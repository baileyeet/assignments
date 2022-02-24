#include <stdio.h>
#include <stdlib.h>

// readvector opens and reads the given file
// The size of the vector should be stored in the parameter size
// The values should be returned in a flat float array
struct vector {
	double one;
	//	double two;
	//	double three;

};

void readvector(const char* filename, int *size) {
	FILE *fp;
	fp = fopen(filename,"r");
	struct vector* v;
	//	v = malloc(sizeof(struct vector));
	//char current_word[200];
	if(fp == NULL) { // check if successful
		perror("Error in opening file");
		exit(1);
	}
	fscanf(fp, "%d", size);
	v = malloc(sizeof(struct vector));
	while (*size > 0) {
		fscanf(fp, "%lf", &v->one);
		 printf("%f\n", v->one);
		*size = *size - 1;
	}
	free(v);
	v = NULL;	
}

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: readvector <filename>\n");
		exit(0);
	}

	int size = 0;
	readvector(argv[1], &size);
	// todo: print vector values
	return 0;
}

