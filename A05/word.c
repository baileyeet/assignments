#include <stdlib.h>
#include <stdio.h>

int main() {
	char arr[4];
	printf("Enter 4 characters: ");
	scanf("%s", arr);
		
	int word = 0;
	int counter = 0;
	for (int i = 3; i >= 0; i--) {
		unsigned int temp = arr[i] << counter;
		printf("Debug %c = 0x%08x\n", arr[i], temp);
		counter += 8;
		word = word | temp;	
	}
	printf("Your number is: %d (0x%08x)\n", word, word);
	return 0;
}
