#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char name[32];
 	float cost;
	int quantity;
}snack;

void print_options(snack*, int);
void create_snack(snack*, int);

int main() {
	int num_snacks;

	printf("Enter a number of snacks: ");
	scanf("%d", &num_snacks);

	snack snackbar[num_snacks];


	for (int i = 0; i < num_snacks; i++) {
		create_snack(snackbar, i);
	}

	print_options(snackbar, num_snacks);

	return 0;
}

void create_snack(snack* snackbar, int index) {
	char name[32];
        double cost;
        int quantity;

	printf("Enter a name: ");
	scanf("%s", name);

	printf("Enter a cost: ");
	scanf("%lf", &cost);

	printf("Enter a quantity: ");
	scanf("%d", &quantity);

	strcpy(snackbar[index].name, name);
	snackbar[index].cost = cost;
	snackbar[index].quantity = quantity;

}

// iterates through snackbar and organizes/prints info
// @param snackbar snack array
void print_options(snack* snackbar, int size){
        printf("Welcome to Dynamic Donna's Snack Bar.\n\n");
	for (int i = 0; i < size; i++) {
                printf("%d) %s\n cost: $%f quantity: %d\n", i, snackbar[i].name, snackbar[i].cost, snackbar[i].quantity);
        printf("\n");
        }
}
