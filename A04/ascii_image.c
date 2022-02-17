#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>

char intensity(int r, int g, int b) {
	int i = (r + g + b)/3;
	if (0 <= i && i <=25) {
		return '@';
	} else if (26 <= i && i <= 50) {
		return '#';
	} else if (51 <= i && i <= 75) {
                return '%';
        } else if (76 <= i && i <= 100) {
                return '*';
        } else if (101 <= i && i <= 125) {
                return 'o';
        } else if (126 <= i && i <= 150) {
                return ';';
        } else if (151 <= i && i <= 175) {
                return ':';
        } else if (176 <= i && i <= 200) {
                return ',';
        } else if (201 <= i && i <= 225) {
                return '.';
        } else if (226 <= i && i <= 255) {
                return ' ';
        }
	return 'x';
}

int main(int argc, char** argv) {
	char* file_name;
	int w = 0;
	int h = 0;
	int count = 1;
	
	if (argc < 2) {
		printf("Supply command line arg");
		return -1;
	}
	file_name = argv[1];
	struct ppm_pixel* arr = read_ppm(file_name, &w, &h);	
	printf("Reading %s with width %d and height %d\n", file_name, w, h);

	for (int i = 0; i < w*h; i++) {
       		char result = intensity(arr[i].red, arr[i].green, arr[i].blue);
       		printf("%c", result);
		//printf("(%d, %d, %d)", arr[i].red, arr[i].green, arr[i].blue);
                if(count == w) {
                        printf("\n");
                        count = 0;
                }
                count++;
        }
	
	free(arr);
	return 0;
}

