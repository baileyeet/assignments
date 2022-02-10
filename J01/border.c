#include <stdio.h>
#include <string.h>

int main() {
	char *word;
	char symbol;
	int word_length;

	printf("Please enter a word: ");
	scanf(" %s", word);
	printf("Please enter a symbol: ");
      	scanf(" %c", &symbol);
	
	//printf(
	word_length = strlen(word);
	for (int i = 0; i < word_length + 4; i++) {
		printf("%c", symbol);
	}

	printf("\n%c %s %c\n", symbol, word, symbol);

	for (int i = 0; i < word_length + 4; i++) {
                printf("%c", symbol);
        }
	printf("\n");
	return 0;
}
