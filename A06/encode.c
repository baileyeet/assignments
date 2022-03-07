#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

char* string_to_binary(char* s) {
	if(s == NULL) return 0; /* no input string */
	size_t len = strlen(s);
	char *binary = malloc(len*8 + 1); // each char is one byte (8 bits) and + 1 at the end for null terminator
	binary[0] = '\0';
	for(size_t i = 0; i < len; ++i) {
		char ch = s[i];
		for(int j = 7; j >= 0; --j){
			if(ch & (1 << j)) {
				strcat(binary,"1");
			} else {
				strcat(binary,"0");
			}
		}
	}
	return binary;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Need param");
		return -1;
	}


	char* file = argv[1];
	int w = 0;
	int h = 0;
	struct ppm_pixel* pxs = read_ppm(file, &w, &h);
	printf("Reading %s with a width %d and a height %d\n", file, w, h);
	/*for (int i = 0; i < w * h; i++) {
		printf("%d", pxs[i].red % 2);
		printf("%d", pxs[i].green % 2);
		printf("%d", pxs[i].blue % 2);
	}*/
	int max = (w*h*3)/8 - 1;
	printf("Max number of characters in the image: %d", max);
	printf("\n Enter a phrase: ");
	char* word = "placeholder";
	scanf(" %s", word);
	if (sizeof(word) > max) {
		printf("Exceeded max length");
		return -1;
	}
	/*int c = 1;
	for (int i = 0; i < w*h; i++) {
		printf("(%d, %d, %d)", pxs[i].red, pxs[i].green, pxs[i].blue);
		if(c == w) {
			printf("\n");
			c = 0;
		}
		c++;
	}*/
	printf("\n");
	int count = 0;
	int temp = 0;
	for (int i = 0; i < strlen(string_to_binary(word)); i++) {
		if (i % 3 == 0) {
			temp = string_to_binary(word)[i] - '0' == pxs[count].red % 2;
			//printf("%d", temp);
			if (temp == 0) {
				//printf("!");
				if (pxs[count].red < 255) {
					pxs[count].red += 1;
				} else {
					pxs[count].red -= 1;
				}
			}	
		} else if (i % 3 == 1) {
			temp = string_to_binary(word)[i] - '0' == pxs[count].green % 2;
			//printf("%d", temp);
			if (temp == 0) {
				//printf("!");
			if (pxs[count].green < 255) {
                                        pxs[count].green += 1;
                                } else {
                                        pxs[count].green -= 1;
                                }
			}
		} else { // i % 3 == 2
			temp = string_to_binary(word)[i] - '0' == pxs[count].blue % 2;
			count++;
			//printf("%d", temp);
			if (temp == 0) {
				//printf("!");
				if (pxs[count].blue < 255) {
                                        pxs[count].blue += 1;
                                } else {
                                        pxs[count].blue -= 1;
                                }
			}
		}
	}
	/*printf("DONE\n");
	int c2 = 1;
	for (int i = 0; i < w*h; i++) {
		printf("(%d, %d, %d)", pxs[i].red, pxs[i].green, pxs[i].blue);
		if(c2 == w) {
			printf("\n");
			c2 = 0;
		}
		c2++;
	}*/
	char* write = strtok(file, ".");

        strcat(write, "-glitch.ppm");
        printf("Writing file %s\n", write);
        write_ppm(write, pxs, w, h);
	
	return 0;

}

