#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
	int w = 0;
	int h = 0;
	int count = 0;
	unsigned char str = 0;
	char * file = "tiny_encoded.ppm";
	struct ppm_pixel* arr = read_ppm(file, &w, &h);
	printf("Testing file %s: %d %d\n", file, w, h);
	for (int i = 0; i < w*h; i++) {
		if (count < 8) {
			unsigned char val = arr[i].red % 2;
			str = str | val << (7 - count); 
			count++;
		}
		if (count < 8) {
			unsigned char val = arr[i].green % 2;
                        str = str | val << (7 - count);  
                        count++;
		}	
		if (count < 8) {
			unsigned char val = arr[i].blue % 2;
                        str = str | val << (7 - count);  
			count++;
		}
		if (count == 8) {
			printf("%c", str);
			count = 0;	
			str = 0;	
		}
	}
	printf("\n");
	free(arr);
	arr = NULL;
	return 0;
}

