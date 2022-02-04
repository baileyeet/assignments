#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	// declare variables
	char *word;
	char read_in[32];
	int count;

	// read in word and count
	printf("Enter a word: ");
	scanf("%s", read_in);
	printf("Enter a count: ");
	scanf("%d", &count);	

	// allocate memory from heap for word
        word = malloc(sizeof(read_in) * count);

        // check if malloc is successful
        if (word == NULL) {
                printf("Error: malloc failed\n");
                exit(1);
        }

	// concatenate for count - 1 times for a total of count copies
	for (int i = 0; i < count; i++) {
		strcat(word, read_in);
	}

	// print out new word
	printf("New word: %s\n", word);

	// reclaim memory and set word to NULL
	free(word);
	word = NULL;

	return 0;
}
