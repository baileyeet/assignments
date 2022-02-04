#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main() {

	FILE *fp;
	char word[32];
	char current_word[32];
	int counter = 0;
	char guess;
	int random_word;
	char stored_guesses[100]; // assumes 100 guesses max
	int tries = 1;
	srand(time(NULL));
	int done = 0;

	fp = fopen("words.txt","r");
	if(fp == NULL) { // check if successful
		perror("Error in opening file");
		return -1;
	}

	random_word = rand() % 68999 + 1; // ignore 0 since not word

	// get until get random_word 
	while(fgets(current_word, sizeof(current_word), fp) && counter != random_word) {
		strcpy(word, current_word);
		counter++;
	}
	fclose(fp);

	//printf("Random word: %s\n", word);

	// print blank spaces for word
	for (int i = 0; i < strlen(word) - 1; i++) {
		printf("_ ");
	}	

	while (done == 0) { // while not done iterate
		printf("\nTurn: %d", tries);
		tries++;
		printf("\nGuess a character :");
		scanf(" %c", &guess);

		strcat(stored_guesses, &guess);
		int matches = 0;

		for (int i = 0; i < strlen(word) - 1; i++) {
			if(strchr(stored_guesses, word[i]) != NULL)
			{
				printf("%c ", word[i]);
				matches++;
			} else {
				printf("_ ");
			}
		}
		if (matches == strlen(word) - 1) {
			done = 1;
			tries -=1;
		}

	}

	printf("\nYou won in %d tries!\n", tries);
	return 0;

}
