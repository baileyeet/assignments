#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
	FILE *fp;
	struct ppm_pixel* arr;
	char current_word[200];
	unsigned char r, g, b;
	int tempw, temph;

	fp = fopen(filename,"r");
	if(fp == NULL) { // check if successful
		perror("Error in opening file");
		return NULL;
	}

	for (int i = 0; i < 2; i++) {
		fgets(current_word, sizeof(current_word), fp);
	}
	sscanf(current_word, "%d %d\n", &tempw, &temph);
	if (current_word[0] == '#') {
		fgets(current_word, sizeof(current_word), fp);
		sscanf(current_word, "%d %d\n", &tempw, &temph);
	}	
	*w = tempw;
	*h = temph;
	
	arr = malloc(sizeof(struct ppm_pixel) * *w * *h);
        if (arr == NULL) {
                printf("Error: malloc failed\n");
                return NULL;
        }

	fgets(current_word, sizeof(current_word), fp);
	for (int i = 0; i < *w * *h; i++) {
		fscanf(fp, " %hhu %hhu %hhu", &r, &g, &b);
                arr[i].red = r;
                arr[i].green = g;
                arr[i].blue = b;	
	}
	fclose(fp);

	return arr;
}

