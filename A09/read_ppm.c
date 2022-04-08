#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
        FILE *fp;
        struct ppm_pixel* arr;
        char current_word[200];
        int tempw, temph;

        fp = fopen(filename,"rb");
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
        fgets(current_word, sizeof(current_word), fp);
        *w = tempw;
        *h = temph;

        arr = malloc(sizeof(struct ppm_pixel) * *w * *h);
        if (arr == NULL) {
                printf("Error: malloc failed\n");
                return NULL;
        }

        fread(arr, sizeof(struct ppm_pixel), *w * *h, fp);

        fclose(fp);

        return arr;
}

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
        FILE *fp;
        fp = fopen(filename, "wb");
	char buffer[256];
	sprintf(buffer, "P6\n%d %d\n255\n", w, h);
	fwrite(buffer, sizeof(char), strlen(buffer), fp);
        fwrite(pixels, sizeof(struct ppm_pixel), w * h, fp);
        fclose(fp);
}

