//---------------------------------------------------------------------
// magic_square.c 
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name:
//
#include <stdio.h>
#include <stdlib.h>

int main() {

	int rows;
	int cols;
	int *arr;
	scanf("%d", &rows);
	scanf("%d", &cols);
	arr = malloc(sizeof(int) * rows * cols);
	if (arr == NULL) {
		printf("Error: malloc failed\n");
		return -1;
	}

	for (int i = 0; i < rows * cols; i++) {
		scanf("%d", &arr[i]);
	}

	// print out 2D array
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			printf("%d ", arr[i * cols + j]);
		printf("\n");
	}
	if (rows == cols) {
		int diag_sum = 0;
		int target_sum_r;
		int target_sum_c;
		int sum = 0;
		for (int i = 0; i < cols; i++) { // check diag
			diag_sum = diag_sum + arr[i * cols + i];
		}
		for (int i = 0; i < rows; i++) { // check rows
			target_sum_r = 0;
			for (int j = 0; j < cols; j++){
				target_sum_r += arr[i * cols + j];
			}
			if (target_sum_r != diag_sum) {
				printf("M is NOT a magic square\n");
				free(arr);
				arr = NULL;
				return 0;

			}
		}
		for (int i = 0; i < rows; i++) { // check cols
			target_sum_c = 0;
			for (int j = 0; j < cols; j++){
				target_sum_c += arr[j * cols + i];
			}
			if (target_sum_c != diag_sum) {
				printf("M is NOT a magic square\n");
				free(arr);
				arr = NULL;
				return 0;
			}
		}
		printf("M is a magic square (magic constant = %d)\n", diag_sum);
		free(arr);
		arr = NULL;
		return 0;	
	}
	// if don't have equal rows cols not magic
	printf("M is NOT a magic square\n");
	free(arr);
	arr = NULL;
	return 0;
}

