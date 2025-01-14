#include <stdio.h>
#include <string.h>
#include "read_ppm.h"
#include <stdlib.h>

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

extern void write_ppm(const char* filename, struct ppm_pixel* pxs, int w, int h) {
	FILE *fp;
	fp = fopen(filename, "wb");
        
	fwrite(pxs, sizeof(struct ppm_pixel), w * h, fp);
	fclose(fp);

}
