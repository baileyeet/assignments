<<<<<<< HEAD
/***************************************************i
 * rpc.c
 * Author: Bailey Hirota
 * Date: 1/22/2022
 * Play rock, paper, scissors
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// methods defined later in code
int playRound(int, int, int);
void printScore(int, int);
void printWinner(int, int);

int main() {
	int max; // max = num trials
	printf("Welcome to Rock, Paper, Scissors!\n How many rounds to you want to play? ");
	scanf("%d", &max);
	playRound(0, 0, max);

	return 0;
}

// play [rounds] rounds for rock paper scissors
// @param int AI score tracker
// @param int player score tracker
// @params number of rounds
int playRound(int aiScore, int playerScore, int rounds) {
	srand(time(NULL));
	char userChoice[50]; // user choice (r,p,s)

	for (int i = 0; i < rounds; i++) {

		printf("Which do you choose? rock, paper, or scissors? ");

		// reading string
		scanf("%s", userChoice);

                printf("Round %d\n", i);

		int r = rand() % 3; //generates 0, 1, or 2

		// 0 = rock
		if (r == 0){
			printf("AI chose rock.\n");
			// If user chooses paper, wins
			if (strcmp(userChoice, "paper") == 0) {
				printf("Paper bashes rock.\n");
				playerScore++;
				// If user chooses, scissors, looses
			} else if (strcmp(userChoice, "scissors") == 0) {
				printf("Rock bashes scissors.\n");
				aiScore++;
				// If user chooses rock, tie
			} else if (strcmp(userChoice, "rock") == 0) {
				printf("Tie.\n");
			} else {
				printf("You entered an invalid choice: %s", userChoice);
			}
		}
		// 1  = paper
		if (r == 1){
			printf("AI chose paper.\n");
			// If user chooses scissors, wins
			if (strcmp(userChoice, "scissors") == 0) {
				printf("Scissors cut  paper.\n");
				playerScore++;
				// If user chooses rock, looses
			} else if (strcmp(userChoice, "rock") == 0) {
				printf("Paper covers  rock.\n");
				aiScore++;
				// If user chooses paper, tie
			} else if (strcmp(userChoice, "paper") == 0) {
				printf("Tie.\n");
			} else {
				printf("You entered an invalid choice: %s", userChoice);
			}
		}
		// 2 = scissors
		if (r == 2){
			printf("AI chose scissors.\n");
			// If user chooses rockr, wins
			if (strcmp(userChoice, "rock") == 0) {
				printf("Rock bashes scissors.\n");
				playerScore++;
				// If user chooses paper, looses
			} else if (strcmp(userChoice, "paper") == 0) {
				printf("Scissors cut paper.\n");
				aiScore++;
				// If user chooses scissors, tie
			} else if (strcmp(userChoice, "scissors") == 0) {
				printf("Tie.\n");
			} else {
				printf("You entered an invalid choice: %s\n", userChoice);
			}
		}
		printScore(aiScore, playerScore);
	}
	printWinner(aiScore, playerScore);
}

// print score for each player
// @param int ai score
// @param int player score
void printScore(int aiScore, int playerScore) {
	printf("AI score: %d, Player score: %d\n", aiScore, playerScore);
}

// calculate winner and print
// @param int ai score
// @param int player score
void printWinner(int aiScore, int playerScore) {
	if (aiScore > playerScore) {
		printf("AI wins!\n");
	} else if (playerScore > aiScore) {
		printf("Player wins!\n");
	} else {
		printf("Tie.\n");
	}
}
=======
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(0));
  return 0;
}

>>>>>>> 0baaa1267aee03906be3cf0a9e84b755a7692fbd
