  /* 
   * Desc:
   *    Program 2 for Assignment 1. Allows user to choose snack from snackbar
   *    and gives according feedback. 
   * @author Bailey Hirota
   * Modified: 1/26/2022
   */

#include <stdio.h>
#include <string.h>
#include <stddef.h>

// snack struct - contains name, cost, and quantity
typedef struct {
   char name[50];
   double cost;
   int   quantity;
} snack;

// global vars 
double money;
int userSnack;
snack snackbar[3];

// functions defined further
void printOptions(snack*);
void canBuy(int, double, snack*);

// initializes snackbar array
// prompts user to select snack and enter $
// prints results
int main(){
    // set names
    strcpy(snackbar[0].name, "Fresh Lychee");
    strcpy(snackbar[1].name, "Yakisoba");
    strcpy(snackbar[2].name, "Taiyaki");
    // set cost
    snackbar[0].cost = 1.25;
    snackbar[1].cost = 4.5;
    snackbar[2].cost = 2.5;
    // set quantities
    snackbar[0].quantity = 0;
    snackbar[1].quantity = 6;
    snackbar[2].quantity = 4;

    printf("Welcome to Bailey's Snack Bar.\n\nHow much money do you have? ");
    scanf("%lf", &money);
  
    printOptions(snackbar);

    printf("What snack would you like to buy? [0, 1, 2] ");
    scanf("%d", &userSnack);

    canBuy(userSnack, money, snackbar);

    return 0;
}

// checks choice and evaluates whether snack can be bought
// @param int user choice
// @param double how much money user has
// @param snack array
void canBuy(int choice, double money, snack* snackbar){
	if (choice == 0) {
		if (snackbar[0].quantity > 1 && money > snackbar[0].cost) { // if able to buy
			printf("You bought %s", snackbar[0].name);
			money -= snackbar[0].cost;
			printf("You have $%f left\n", money);
		} else {printf("Cannot buy\n");} // not able to buy
	} else if (choice == 1) {
                if (snackbar[1].quantity > 1 && money > snackbar[1].cost) { // can buy
                        printf("You bought %s\n", snackbar[1].name);
                        money -= snackbar[1].cost;
                        printf("You have $%f left\n", money);
                } else {printf("Cannot buy\n");} // can't buy
        } else if (choice == 2) {
                if (snackbar[2].quantity > 1 && money > snackbar[2].cost) { // can buy
                        printf("You bought %s\n", snackbar[2].name);
                        money -= snackbar[2].cost;
                        printf("You have $%f left\n", money);
                } else {printf("Cannot buy\n");} // cannot buy
        } else {printf("Invalid input.\n");}
}
// iterates through snackbar and organizes/prints info
// @param snackbar snack array
void printOptions(snack* snackbar){
	for (int i = 0; i < 3; i++) {
		printf("%d) %s\n cost: $%f quantity: %d\n", i, snackbar[i].name, snackbar[i].cost, snackbar[i].quantity);
	printf("\n");
	}
}
